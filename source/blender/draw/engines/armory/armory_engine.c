/*
 * Copyright 2016, Blender Foundation.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * Contributor(s): Blender Institute
 *
 */

/** \file armory_engine.c
 *  \ingroup draw_engine
 */

#include "DRW_render.h"

#include "BKE_icons.h"
#include "BKE_idprop.h"
#include "BKE_main.h"
#include "BKE_particle.h"

#include "BKE_context.h"
#include "BKE_screen.h"
#include "ED_space_api.h"
#include "ED_screen.h"

#include "WM_api.h"
#include "WM_types.h"

#include "DNA_particle_types.h"
#include "GPU_shader.h"

#include "armory_engine.h"
#include "Krom.h"

#define ARMORY_ENGINE "ARMORY"

/* *********** LISTS *********** */

/* GPUViewport.storage
 * Is freed everytime the viewport engine changes */
typedef struct ARMORY_StorageList {
	struct ARMORY_PrivateData *g_data;
} ARMORY_StorageList;

typedef struct ARMORY_PassList {
	struct DRWPass *color_pass;
} ARMORY_PassList;

typedef struct ARMORY_Data {
	void *engine_type;
	DRWViewportEmptyList *fbl;
	DRWViewportEmptyList *txl;
	ARMORY_PassList *psl;
	ARMORY_StorageList *stl;
} ARMORY_Data;

/* *********** STATIC *********** */

static struct {
	/* Shading Pass */
	struct GPUShader *color_sh;
} e_data = {NULL}; /* Engine data */

typedef struct ARMORY_PrivateData {
	DRWShadingGroup *color_shgrp;
} ARMORY_PrivateData; /* Transient data */

/* Functions */

static void armory_engine_init(void *UNUSED(vedata))
{

}

static void armory_cache_init(void *vedata)
{
	ARMORY_PassList *psl = ((ARMORY_Data *)vedata)->psl;
	ARMORY_StorageList *stl = ((ARMORY_Data *)vedata)->stl;
}

static void armory_cache_populate(void *vedata, Object *ob)
{
	ARMORY_StorageList *stl = ((ARMORY_Data *)vedata)->stl;

	if (!DRW_object_is_renderable(ob)) {
		return;
	}

	const DRWContextState *draw_ctx = DRW_context_state_get();
	if (ob == draw_ctx->object_edit) {
		return;
	}
}

static void armory_cache_finish(void *vedata)
{
	ARMORY_StorageList *stl = ((ARMORY_Data *)vedata)->stl;
	UNUSED_VARS(stl);
}

// Input
bool keystate[512];
bool first = true;
int lastmx = -1, lastmy = -1;
int pressed = 0;
int lasteval = -1;
int lastetype = -1;

static void handleMouse(int mx, int my, int etype, int eval) {

    if (etype == LEFTMOUSE || etype == RIGHTMOUSE) {
		int button = etype == LEFTMOUSE ? 0 : 1;
		if (eval == KM_PRESS && pressed == 0) {
			pressed = 1;
			armoryMousePress(button, mx, my);	
		}
		else if (eval == KM_RELEASE && pressed == 1) {
			pressed = 0;
			armoryMouseRelease(button, mx, my);
		}
	}
}

static void handleMousePos(int mx, int my) {
	if (mx != lastmx || my != lastmy) {
		armoryMouseMove(mx, my);
	}
}

static bool isKey(int etype) {
	// wm_event_types.h
	return (etype >= LEFTARROWKEY && etype <= UPARROWKEY) ||
		(etype >= AKEY && etype <= ZKEY) ||
		(etype >= ZEROKEY && etype <= NINEKEY) ||
		(etype == LEFTSHIFTKEY || etype == ESCKEY);
}

static void releaseKeys() {
	for (int i = LEFTARROWKEY; i <= UPARROWKEY; i++) {
		if (keystate[i]) { keystate[i] = false; armoryKeyUp(i); }
	}
	for (int i = AKEY; i <= ZKEY; i++) {
		if (keystate[i]) { keystate[i] = false; armoryKeyUp(i); }
	}
	for (int i = ZEROKEY; i <= NINEKEY; i++) {
		if (keystate[i]) { keystate[i] = false; armoryKeyUp(i); }
	}
	if (keystate[LEFTSHIFTKEY]) { keystate[LEFTSHIFTKEY] = false; armoryKeyUp(LEFTSHIFTKEY); }
	if (keystate[ESCKEY]) { keystate[ESCKEY] = false; armoryKeyUp(ESCKEY); }
}

static void handleKey(int etype, int eval) {

	if (eval == KM_PRESS && keystate[etype] == false) {
		armoryKeyDown(etype);
		keystate[etype] = true;
	}
	else if (eval == KM_RELEASE && keystate[etype]) {
		armoryKeyUp(etype);
		keystate[etype] = false;
	}
}

static void armory_draw_scene(void *vedata)
{
	if (first) {
		first = false;
		for (int i = 0; i < 512; i++) keystate[i] = false;
	}

	const DRWContextState *draw_ctx = DRW_context_state_get();
	const bContext *C = draw_ctx->evil_C;
	ARegion *ar = draw_ctx->ar;

	// Hacky input detect
	{
		int x = ar->winrct.xmin;
		int xmax = ar->winrct.xmax;
		int y = ar->winrct.ymin;
		int ymax = ar->winrct.ymax;
		int w = ar->winrct.xmax - ar->winrct.xmin;
		int h = ar->winrct.ymax - ar->winrct.ymin;
		wmWindow *win = CTX_wm_window(C);
		wmEvent* event = win->eventstate;

		int mx = event->x - x;
		int my = ymax - event->y;

		// Mouse in bounds
		if (event->x >= x && event->x <= xmax && event->y >= y && event->y <= ymax) {
			handleMousePos(mx, my);
	        // Prev event unhandled, > 2 events per frame still get unhandled
			if (event->prevval == KM_PRESS || event->prevval == KM_RELEASE) {
				if (isKey(event->prevtype)) {
					if (event->prevtype != lastetype) {
						if (event->prevtype != event->type) {
							handleKey(event->prevtype, event->prevval);
						}
					}
				}
				else handleMouse(mx, my, event->type, event->val);
			}
			if (event->val == KM_PRESS || event->val == KM_RELEASE) {
				if (isKey(event->type)) {
					if (event->type != lastetype || event->val != lasteval) {
						handleKey(event->type, event->val);
					}
				}
				else handleMouse(mx, my, event->type, event->val);
			}
			// 2 keys released at once, release all keys to prevent key stuck
			if (event->prevval == KM_RELEASE && event->val == KM_RELEASE && event->prevtype != event->type) {
				releaseKeys();
			}
		}

		lastetype = event->type;
		lasteval = event->val;

		lastmx = mx;
		lastmy = my;
	}

	armoryDraw();

	// Cursor hide
	if (armoryIsMouseLocked()) {
		wmWindow *win = CTX_wm_window(C);
		WM_cursor_set(win, CURSOR_NONE);
	}
	else{
		wmWindow *win = CTX_wm_window(C);
		WM_cursor_set(win, CURSOR_STD);
	}

	// Draw constantly
	ar->do_draw = -1;
}

static void armory_engine_free(void)
{
	/* all shaders are builtin */
}

static const DrawEngineDataSize armory_data_size = DRW_VIEWPORT_DATA_SIZE(ARMORY_Data);

DrawEngineType draw_engine_armory_type = {
	NULL, NULL,
	N_("Armory"),
	&armory_data_size,
	&armory_engine_init,
	&armory_engine_free,
	&armory_cache_init,
	&armory_cache_populate,
	&armory_cache_finish,
	NULL,
	&armory_draw_scene,
	NULL,
	NULL,
	NULL,
};

/* Note: currently unused, we may want to register so we can see this when debugging the view. */

RenderEngineType DRW_engine_viewport_armory_type = {
	NULL, NULL,
	ARMORY_ENGINE, N_("Armory"), RE_INTERNAL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	&draw_engine_armory_type,
	{NULL, NULL, NULL}
};


#undef ARMORY_ENGINE
