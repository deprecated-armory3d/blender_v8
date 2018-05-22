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
 *
 * Simple engine for drawing color and/or depth.
 * When we only need simple flat shaders.
 */

#include "DRW_render.h"

#include "BKE_icons.h"
#include "BKE_idprop.h"
#include "BKE_main.h"
#include "BKE_particle.h"

#include "DNA_particle_types.h"

#include "GPU_shader.h"

#include "armory_engine.h"
/* Shaders */

#define ARMORY_ENGINE "ARMORY"

/* we may want this later? */
#define USE_DEPTH

/* *********** LISTS *********** */

/* GPUViewport.storage
 * Is freed everytime the viewport engine changes */
typedef struct ARMORY_StorageList {
	struct ARMORY_PrivateData *g_data;
} ARMORY_StorageList;

typedef struct ARMORY_PassList {
#ifdef USE_DEPTH
	struct DRWPass *depth_pass;
	struct DRWPass *depth_pass_cull;
#endif
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
#ifdef USE_DEPTH
	/* Depth Pre Pass */
	struct GPUShader *depth_sh;
#endif
	/* Shading Pass */
	struct GPUShader *color_sh;
} e_data = {NULL}; /* Engine data */

typedef struct ARMORY_PrivateData {
#ifdef USE_DEPTH
	DRWShadingGroup *depth_shgrp;
	DRWShadingGroup *depth_shgrp_cull;
	DRWShadingGroup *depth_shgrp_hair;
#endif
	DRWShadingGroup *color_shgrp;
} ARMORY_PrivateData; /* Transient data */

/* Functions */

static void armory_engine_init(void *UNUSED(vedata))
{
#ifdef USE_DEPTH
	/* Depth prepass */
	if (!e_data.depth_sh) {
		e_data.depth_sh = DRW_shader_create_3D_depth_only();
	}
#endif

	/* Shading pass */
	if (!e_data.color_sh) {
		e_data.color_sh = GPU_shader_get_builtin_shader(GPU_SHADER_3D_UNIFORM_COLOR);
	}
}

static void armory_cache_init(void *vedata)
{
	ARMORY_PassList *psl = ((ARMORY_Data *)vedata)->psl;
	ARMORY_StorageList *stl = ((ARMORY_Data *)vedata)->stl;

	if (!stl->g_data) {
		/* Alloc transient pointers */
		stl->g_data = MEM_mallocN(sizeof(*stl->g_data), __func__);
	}

#ifdef USE_DEPTH
	/* Depth Pass */
	{
		psl->depth_pass = DRW_pass_create("Depth Pass", DRW_STATE_WRITE_DEPTH | DRW_STATE_DEPTH_LESS | DRW_STATE_WIRE);
		stl->g_data->depth_shgrp = DRW_shgroup_create(e_data.depth_sh, psl->depth_pass);

		psl->depth_pass_cull = DRW_pass_create(
		        "Depth Pass Cull",
		        DRW_STATE_WRITE_DEPTH | DRW_STATE_DEPTH_LESS | DRW_STATE_CULL_BACK);
		stl->g_data->depth_shgrp_cull = DRW_shgroup_create(e_data.depth_sh, psl->depth_pass_cull);
	}
#endif

	/* Color Pass */
	{
		psl->color_pass = DRW_pass_create("Color Pass", DRW_STATE_WRITE_COLOR | DRW_STATE_DEPTH_EQUAL);
		stl->g_data->color_shgrp = DRW_shgroup_create(e_data.color_sh, psl->color_pass);
	}
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

	for (ParticleSystem *psys = ob->particlesystem.first; psys; psys = psys->next) {
		if (!psys_check_enabled(ob, psys, false)) {
			continue;
		}
		if (!DRW_check_psys_visible_within_active_context(ob, psys)) {
			return;
		}
		ParticleSettings *part = psys->part;
		const int draw_as = (part->draw_as == PART_DRAW_REND) ? part->ren_as : part->draw_as;
		if (draw_as == PART_DRAW_PATH) {
			struct Gwn_Batch *hairs = DRW_cache_particles_get_hair(ob, psys, NULL);
			#ifdef USE_DEPTH // TODO: ARMORY
			DRW_shgroup_call_add(stl->g_data->depth_shgrp, hairs, NULL);
			#endif
		}
	}

	struct Gwn_Batch *geom = DRW_cache_object_surface_get(ob);
	if (geom) {
		bool do_cull = false;  /* TODO (we probably wan't to take this from the viewport?) */
#ifdef USE_DEPTH
		/* Depth Prepass */
		DRW_shgroup_call_add((do_cull) ? stl->g_data->depth_shgrp_cull : stl->g_data->depth_shgrp, geom, ob->obmat);
#endif
		/* Shading */
		DRW_shgroup_call_add(stl->g_data->color_shgrp, geom, ob->obmat);
	}
}

static void armory_cache_finish(void *vedata)
{
	ARMORY_StorageList *stl = ((ARMORY_Data *)vedata)->stl;

	UNUSED_VARS(stl);
}

static void armory_draw_scene(void *vedata)
{

	ARMORY_PassList *psl = ((ARMORY_Data *)vedata)->psl;
	const bool is_select = DRW_state_is_select();

	bool use_color = true;
	bool use_depth = true;
	bool use_depth_cull = true;

	if (is_select) {
		/* Needed for depth-picking,
		 * for other selection types there are no need for extra passes either. */
		use_color = false;
		use_depth_cull = false;
	}

#ifdef USE_DEPTH
	/* Pass 1 : Depth pre-pass */
	if (use_depth) {
		DRW_draw_pass(psl->depth_pass);
	}

	if (use_depth_cull) {
		DRW_draw_pass(psl->depth_pass_cull);
	}
#endif

	/* Pass 3 : Shading */
	if (use_color) {
		DRW_draw_pass(psl->color_pass);
	}
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
