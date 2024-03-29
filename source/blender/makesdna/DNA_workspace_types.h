/*
 * ***** BEGIN GPL LICENSE BLOCK *****
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
 * ***** END GPL LICENSE BLOCK *****
 */

/** \file DNA_workspace_types.h
 *  \ingroup DNA
 *
 * Use API in BKE_workspace.h!
 * Struct members marked with DNA_PRIVATE_WORKSPACE will throw a
 * warning saying it's deprecated when used outside of workspace.c.
 */

#ifndef __DNA_WORKSPACE_TYPES_H__
#define __DNA_WORKSPACE_TYPES_H__

#include "DNA_scene_types.h"

/* Same logic as DNA_DEPRECATED_ALLOW, but throws 'deprecated'
 * warnings if DNA_PRIVATE_WORKSPACE_ALLOW is not defined */
#ifdef DNA_PRIVATE_WORKSPACE_ALLOW
   /* allow use of private items */
#  define DNA_PRIVATE_WORKSPACE
#else
#  ifndef DNA_PRIVATE_WORKSPACE
#    define DNA_PRIVATE_WORKSPACE DNA_PRIVATE_ATTR
#  endif
#endif

#ifdef DNA_PRIVATE_READ_WRITE_ALLOW
#  define DNA_PRIVATE_WORKSPACE_READ_WRITE
#else
#  ifndef DNA_PRIVATE_WORKSPACE_READ_WRITE
#    define DNA_PRIVATE_WORKSPACE_READ_WRITE DNA_PRIVATE_WORKSPACE
#  endif
#endif

/* Currently testing, allow to disable. */
#define USE_WORKSPACE_TOOL

#
#
typedef struct bToolRef_Runtime {
	int cursor;

	/** One of these 3 must be defined. */
	char keymap[64];
	char gizmo_group[64];
	char data_block[64];

	/** Use to infer primary operator to use when setting accelerator keys. */
	char operator[64];

	/** Index when a tool is a member of a group. */
	int index;
} bToolRef_Runtime;


/* Stored per mode. */
typedef struct bToolRef {
	struct bToolRef *next, *prev;
	char idname[64];

	/** Use to avoid initializing the same tool multiple times. */
	short tag;

	/** #bToolKey (spacetype, mode), used in 'WM_api.h' */
	short space_type;
	/**
	 * Value depends ont the 'space_type', object mode for 3D view, image editor has own mode too.
	 * RNA needs to handle using item function.
	 */
	int mode;

	/**
	 * Use for tool options, each group's name must match a tool name:
	 *
	 *    {"Tool Name": {"SOME_OT_operator": {...}, ..}, ..}
	 *
	 * This is done since different tools may call the same operators with their own options.
	 */
	IDProperty *properties;

	/** Variables needed to operate the tool. */
	bToolRef_Runtime *runtime;
} bToolRef;


/**
 * \brief Wrapper for bScreen.
 *
 * bScreens are IDs and thus stored in a main list-base. We also want to store a list-base of them within the
 * workspace (so each workspace can have its own set of screen-layouts) which would mess with the next/prev pointers.
 * So we use this struct to wrap a bScreen pointer with another pair of next/prev pointers.
 */
typedef struct WorkSpaceLayout {
	struct WorkSpaceLayout *next, *prev;

	struct bScreen *screen DNA_PRIVATE_WORKSPACE;
	/* The name of this layout, we override the RNA name of the screen with this (but not ID name itself) */
	char name[64] DNA_PRIVATE_WORKSPACE; /* MAX_NAME */
} WorkSpaceLayout;

/** Optional tags, which features to use, aligned with #bAddon names by convention. */
typedef struct wmOwnerID {
	struct wmOwnerID *next, *prev;
	char name[64] DNA_PRIVATE_WORKSPACE; /* MAX_NAME */
} wmOwnerID;

typedef struct WorkSpace {
	ID id;

	ListBase layouts DNA_PRIVATE_WORKSPACE; /* WorkSpaceLayout */
	/* Store for each hook (so for each window) which layout has
	 * been activated the last time this workspace was visible. */
	ListBase hook_layout_relations DNA_PRIVATE_WORKSPACE_READ_WRITE; /* WorkSpaceDataRelation */

	/* Feature tagging (use for addons) */
	ListBase owner_ids DNA_PRIVATE_WORKSPACE_READ_WRITE; /* wmOwnerID */

	/* should be: '#ifdef USE_WORKSPACE_TOOL'. */

	/** List of #bToolRef */
	ListBase tools;

	/**
	 * BAD DESIGN WARNING:
	 * This is a workaround for the topbar not knowing which tools spac */
	char tools_space_type;
	/** Type is different for each space-type. */
	char tools_mode;
	char _pad[6];

	int object_mode;

	int flags DNA_PRIVATE_WORKSPACE; /* enum eWorkSpaceFlags */

	/* Info text from modal operators (runtime). */
	char *status_text;
} WorkSpace;

/* internal struct, but exported for read/write */
#if defined(DNA_PRIVATE_READ_WRITE_ALLOW) || defined(DNA_PRIVATE_WORKSPACE_ALLOW)

/**
 * Generic (and simple/primitive) struct for storing a history of assignments/relations
 * of workspace data to non-workspace data in a listbase inside the workspace.
 *
 * Using this we can restore the old state of a workspace if the user switches back to it.
 *
 * Usage
 * =====
 * When activating a workspace, it should activate the screen-layout that was active in that
 * workspace before *in this window*.
 * More concretely:
 * * There are two windows, win1 and win2.
 * * Both show workspace ws1, but both also had workspace ws2 activated at some point before.
 * * Last time ws2 was active in win1, screen-layout sl1 was activated.
 * * Last time ws2 was active in win2, screen-layout sl2 was activated.
 * * When changing from ws1 to ws2 in win1, screen-layout sl1 should be activated again.
 * * When changing from ws1 to ws2 in win2, screen-layout sl2 should be activated again.
 * So that means we have to store the active screen-layout in a per workspace, per window
 * relation. This struct is used to store an active screen-layout for each window within the
 * workspace.
 * To find the screen-layout to activate for this window-workspace combination, simply lookup
 * the WorkSpaceDataRelation with the workspace-hook of the window set as parent.
 */
typedef struct WorkSpaceDataRelation {
	struct WorkSpaceDataRelation *next, *prev;

	/* the data used to identify the relation (e.g. to find screen-layout (= value) from/for a hook) */
	void *parent;
	/* The value for this parent-data/workspace relation */
	void *value;
} WorkSpaceDataRelation;

#endif /* DNA_PRIVATE_WORKSPACE_READ_WRITE */

/**
 * Little wrapper to store data that is going to be per window, but comming from the workspace.
 * It allows us to keep workspace and window data completely separate.
 */
typedef struct WorkSpaceInstanceHook {
	WorkSpace *active DNA_PRIVATE_WORKSPACE;
	struct WorkSpaceLayout *act_layout DNA_PRIVATE_WORKSPACE;

	/* Needed because we can't change workspaces/layouts in running handler loop, it would break context. */
	WorkSpace *temp_workspace_store;
	struct WorkSpaceLayout *temp_layout_store;
} WorkSpaceInstanceHook;

typedef enum eWorkSpaceFlags {
	WORKSPACE_USE_FILTER_BY_ORIGIN = (1 << 1),
} eWorkSpaceFlags;

#endif /* __DNA_WORKSPACE_TYPES_H__ */
