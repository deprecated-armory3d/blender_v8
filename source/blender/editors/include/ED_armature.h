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
 * The Original Code is Copyright (C) 2001-2002 by NaN Holding BV.
 * All rights reserved.
 *
 * Contributor(s): Blender Foundation
 *
 * ***** END GPL LICENSE BLOCK *****
 */

/** \file ED_armature.h
 *  \ingroup editors
 */

#ifndef __ED_ARMATURE_H__
#define __ED_ARMATURE_H__

#ifdef __cplusplus
extern "C" {
#endif

struct bArmature;
struct Base;
struct bContext;
struct Bone;
struct bPoseChannel;
struct Depsgraph;
struct IDProperty;
struct ListBase;
struct Main;
struct MeshDeformModifierData;
struct Mesh;
struct Object;
struct ReportList;
struct Scene;
struct ViewLayer;
struct ViewContext;
struct wmKeyConfig;
struct wmOperator;
struct Main;
struct UndoType;

typedef struct EditBone {
	struct EditBone *next, *prev;
	struct IDProperty *prop; /* User-Defined Properties on this Bone */
	struct EditBone *parent; /* Editbones have a one-way link  (i.e. children refer
	                          * to parents.  This is converted to a two-way link for
	                          * normal bones when leaving editmode. */
	char    name[64];       /* MAXBONENAME */
	float   roll;           /* Roll along axis.  We'll ultimately use the axis/angle method
	                         * for determining the transformation matrix of the bone.  The axis
	                         * is tail-head while roll provides the angle. Refer to Graphics
	                         * Gems 1 p. 466 (section IX.6) if it's not already in here somewhere*/

	float head[3];          /* Orientation and length is implicit during editing */
	float tail[3];
	/* All joints are considered to have zero rotation with respect to
	 * their parents.	Therefore any rotations specified during the
	 * animation are automatically relative to the bones' rest positions*/
	int flag;
	int layer;

	float dist, weight;
	float xwidth, length, zwidth;  /* put them in order! transform uses this as scale */
	float rad_head, rad_tail;

	/* Bendy-Bone parameters */
	float roll1, roll2;
	float curveOutX, curveOutY;
	float curveInX, curveInY;
	float ease1, ease2;
	float scaleIn, scaleOut;

	float oldlength;        /* for envelope scaling */

	short segments;

	/* Used for display */
	float disp_mat[4][4];  /*  in Armature space, rest pos matrix */
	float disp_tail_mat[4][4];  /*  in Armature space, rest pos matrix */
	/* 32 == MAX_BBONE_SUBDIV */
	float disp_bbone_mat[32][4][4]; /*  in Armature space, rest pos matrix */

	/* Used to store temporary data */
	union {
		struct EditBone *ebone;
		struct Bone     *bone;
		void            *p;
		int              i;
	} temp;
} EditBone;

#define BONESEL_ROOT    (1 << 28)
#define BONESEL_TIP     (1 << 29)
#define BONESEL_BONE    (1 << 30)
#define BONESEL_ANY     (BONESEL_TIP | BONESEL_ROOT | BONESEL_BONE)

#define BONESEL_NOSEL   (1u << 31u)

/* useful macros */
#define EBONE_VISIBLE(arm, ebone) ( \
	CHECK_TYPE_INLINE(arm, bArmature *), \
	CHECK_TYPE_INLINE(ebone, EditBone *), \
	(((arm)->layer & (ebone)->layer) && !((ebone)->flag & BONE_HIDDEN_A)) \
	)

#define EBONE_SELECTABLE(arm, ebone) (EBONE_VISIBLE(arm, ebone) && !(ebone->flag & BONE_UNSELECTABLE))

#define EBONE_EDITABLE(ebone) ( \
	CHECK_TYPE_INLINE(ebone, EditBone *), \
	(((ebone)->flag & BONE_SELECTED) && !((ebone)->flag & BONE_EDITMODE_LOCKED)) \
	)

/* used in armature_select_hierarchy_exec() */
#define BONE_SELECT_PARENT  0
#define BONE_SELECT_CHILD   1

/* armature_ops.c */
void ED_operatortypes_armature(void);
void ED_operatormacros_armature(void);
void ED_keymap_armature(struct wmKeyConfig *keyconf);

/* editarmature.c */
void ED_armature_from_edit(struct Main *bmain, struct bArmature *arm);
void ED_armature_to_edit(struct bArmature *arm);
void ED_armature_edit_free(struct bArmature *arm);

void ED_armature_edit_deselect_all(struct Object *obedit);
void ED_armature_edit_deselect_all_visible(struct Object *obedit);

void ED_armature_edit_deselect_all_multi(struct Object **objects, uint objects_len);
void ED_armature_edit_deselect_all_visible_multi(struct Object **objects, uint objects_len);

bool ED_armature_pose_select_pick_with_buffer(
        struct ViewLayer *view_layer, struct Base *base, const unsigned int *buffer, short hits,
        bool extend, bool deselect, bool toggle, bool do_nearest);
bool ED_armature_edit_select_pick(
        struct bContext *C, const int mval[2], bool extend, bool deselect, bool toggle);
int join_armature_exec(struct bContext *C, struct wmOperator *op);
float ED_armature_ebone_roll_to_vector(const EditBone *bone, const float new_up_axis[3], const bool axis_only);
EditBone *ED_armature_ebone_find_name(const struct ListBase *edbo, const char *name);
EditBone *ED_armature_ebone_get_mirrored(const struct ListBase *edbo, EditBone *ebo);
void ED_armature_edit_sync_selection(struct ListBase *edbo);
void ED_armature_edit_validate_active(struct bArmature *arm);

struct Base *ED_armature_base_and_ebone_from_select_buffer(
        struct Base **bases, uint bases_len, int hit, struct EditBone **r_ebone);
struct Object *ED_armature_object_and_ebone_from_select_buffer(
        struct Object **objects, uint objects_len, int hit, struct EditBone **r_ebone);

struct Base *ED_armature_base_and_bone_from_select_buffer(
        struct Base **bases, uint bases_len, int hit, struct Bone **r_bone);

EditBone *ED_armature_ebone_add_primitive(struct Object *obedit_arm, float length, bool view_aligned);
EditBone *ED_armature_ebone_add(struct bArmature *arm, const char *name);

void ED_armature_ebone_remove_ex(struct bArmature *arm, EditBone *exBone, bool clear_connected);
void ED_armature_ebone_remove(struct bArmature *arm, EditBone *exBone);

bool ED_armature_ebone_is_child_recursive(EditBone *ebone_parent, EditBone *ebone_child);
EditBone *ED_armature_ebone_find_shared_parent(EditBone *ebone_child[], const unsigned int ebone_child_tot);

void ED_armature_ebone_to_mat3(EditBone *ebone, float mat[3][3]);
void ED_armature_ebone_to_mat4(EditBone *ebone, float mat[4][4]);

void ED_armature_ebone_from_mat3(EditBone *ebone, float mat[3][3]);
void ED_armature_ebone_from_mat4(EditBone *ebone, float mat[4][4]);

void ED_armature_edit_transform_mirror_update(struct Object *obedit);
void ED_armature_origin_set(struct Main *bmain, struct Object *ob, float cursor[3], int centermode, int around);

void ED_armature_transform_bones(struct bArmature *arm, float mat[4][4], const bool do_props);
void ED_armature_transform_apply(struct Main *bmain, struct Object *ob, float mat[4][4], const bool do_props);
void ED_armature_transform(struct Main *bmain, struct bArmature *arm, float mat[4][4], const bool do_props);

#define ARM_GROUPS_NAME     1
#define ARM_GROUPS_ENVELOPE 2
#define ARM_GROUPS_AUTO     3

void ED_object_vgroup_calc_from_armature(
        struct ReportList *reports, struct Depsgraph *depsgraph, struct Scene *scene,
        struct Object *ob, struct Object *par, const int mode, const bool mirror);

/* if bone is already in list, pass it as param to ignore it */
void ED_armature_ebone_unique_name(struct ListBase *ebones, char *name, EditBone *bone);
void ED_armature_bone_rename(struct Main *bmain, struct bArmature *arm, const char *oldnamep, const char *newnamep);
void ED_armature_bones_flip_names(struct Main *bmain, struct bArmature *arm, struct ListBase *bones_names, const bool do_strip_numbers);

/* low level selection functions which handle */
int  ED_armature_ebone_selectflag_get(const EditBone *ebone);
void ED_armature_ebone_selectflag_set(EditBone *ebone, int flag);
void ED_armature_ebone_select_set(EditBone *ebone, bool select);
void ED_armature_ebone_selectflag_enable(EditBone *ebone, int flag);
void ED_armature_ebone_selectflag_disable(EditBone *ebone, int flag);

/* editarmature_undo.c */
void ED_armature_undosys_type(struct UndoType *ut);

/* armature_utils.c */
void ED_armature_ebone_listbase_temp_clear(struct ListBase *lb);
void ED_armature_ebone_listbase_free(struct ListBase *lb);
void ED_armature_ebone_listbase_copy(struct ListBase *lb_dst, struct ListBase *lb_src);

/* poseobject.c */
bool ED_object_posemode_exit_ex(struct Main *bmain, struct Object *ob);
bool ED_object_posemode_exit(struct bContext *C, struct Object *ob);
bool ED_object_posemode_enter_ex(struct Main *bmain, struct Object *ob);
bool ED_object_posemode_enter(struct bContext *C, struct Object *ob);
bool ED_pose_deselect_all(struct Object *ob, int select_mode, const bool ignore_visibility);
void ED_pose_deselect_all_multi(struct Object **objects, uint objects_len, int select_mode, const bool ignore_visibility);
void ED_pose_bone_select_tag_update(struct Object *ob);
void ED_pose_bone_select(struct Object *ob, struct bPoseChannel *pchan, bool select);
void ED_pose_recalculate_paths(struct bContext *C, struct Scene *scene, struct Object *ob);
struct Object *ED_pose_object_from_context(struct bContext *C);

/* meshlaplacian.c */
void ED_mesh_deform_bind_callback(
        struct Scene *scene,
        struct MeshDeformModifierData *mmd,
        struct Mesh *cagemesh,
        float *vertexcos, int totvert, float cagemat[4][4]);

#ifdef __cplusplus
}
#endif

#endif /* __ED_ARMATURE_H__ */
