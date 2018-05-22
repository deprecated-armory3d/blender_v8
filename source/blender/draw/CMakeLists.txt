# ***** BEGIN GPL LICENSE BLOCK *****
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software Foundation,
# Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
#
# The Original Code is Copyright (C) 2016, Blender Foundation
# All rights reserved.
#
# The Original Code is: all of this file.
#
# Contributor(s): Blender Institute
#
# ***** END GPL LICENSE BLOCK *****

set(INC
	.
	intern
	modes

	../blenfont
	../blenkernel
	../blenlib
	../blentranslation
	../bmesh
	../depsgraph
	../editors/include
	../editors/space_view3d
	../gpu
	../imbuf
	../makesdna
	../makesrna
	../render/extern/include
	../render/intern/include
	../windowmanager

	../../../intern/glew-mx
	../../../intern/guardedalloc
)

set(INC_SYS
	${GLEW_INCLUDE_PATH}
)

set(SRC
	intern/draw_armature.c
	intern/draw_cache.c
	intern/draw_cache_impl_curve.c
	intern/draw_cache_impl_displist.c
	intern/draw_cache_impl_lattice.c
	intern/draw_cache_impl_mesh.c
	intern/draw_cache_impl_metaball.c
	intern/draw_cache_impl_particles.c
	intern/draw_common.c
	intern/draw_instance_data.c
	intern/draw_manager.c
	intern/draw_manager_data.c
	intern/draw_manager_exec.c
	intern/draw_manager_shader.c
	intern/draw_manager_text.c
	intern/draw_manager_texture.c
	intern/draw_manager_profiling.c
	intern/draw_view.c
	modes/edit_armature_mode.c
	modes/edit_curve_mode.c
	modes/edit_lattice_mode.c
	modes/edit_mesh_mode.c
	modes/edit_mesh_mode_text.c
	modes/edit_metaball_mode.c
	modes/edit_surface_mode.c
	modes/edit_text_mode.c
	modes/object_mode.c
	modes/overlay_mode.c
	modes/paint_texture_mode.c
	modes/paint_vertex_mode.c
	modes/paint_weight_mode.c
	modes/particle_mode.c
	modes/pose_mode.c
	modes/sculpt_mode.c
	engines/basic/basic_engine.c
	engines/clay/clay_engine.c
	engines/eevee/eevee_bloom.c
	engines/eevee/eevee_data.c
	engines/eevee/eevee_depth_of_field.c
	engines/eevee/eevee_effects.c
	engines/eevee/eevee_engine.c
	engines/eevee/eevee_lightprobes.c
	engines/eevee/eevee_lights.c
	engines/eevee/eevee_materials.c
	engines/eevee/eevee_mist.c
	engines/eevee/eevee_motion_blur.c
	engines/eevee/eevee_occlusion.c
	engines/eevee/eevee_render.c
	engines/eevee/eevee_screen_raytrace.c
	engines/eevee/eevee_subsurface.c
	engines/eevee/eevee_temporal_sampling.c
	engines/eevee/eevee_volumes.c
	engines/workbench/workbench_engine.c
	engines/workbench/workbench_materials.c
	engines/workbench/workbench_studiolight.c
	engines/workbench/solid_mode.c
	engines/external/external_engine.c

	DRW_engine.h
	intern/DRW_render.h
	intern/draw_cache.h
	intern/draw_cache_impl.h
	intern/draw_common.h
	intern/draw_instance_data.h
	intern/draw_manager.h
	intern/draw_manager_text.h
	intern/draw_manager_profiling.h
	intern/draw_view.h
	modes/draw_mode_engines.h
	modes/edit_mesh_mode_intern.h
	engines/basic/basic_engine.h
	engines/clay/clay_engine.h
	engines/eevee/eevee_engine.h
	engines/eevee/eevee_lut.h
	engines/eevee/eevee_private.h
	engines/external/external_engine.h
)

if(WITH_CLAY_ENGINE)
	add_definitions(-DWITH_CLAY_ENGINE)
endif()

data_to_c_simple(engines/clay/shaders/clay_frag.glsl SRC)
data_to_c_simple(engines/clay/shaders/clay_fxaa.glsl SRC)
data_to_c_simple(engines/clay/shaders/clay_copy.glsl SRC)
data_to_c_simple(engines/clay/shaders/clay_prepass_frag.glsl SRC)
data_to_c_simple(engines/clay/shaders/clay_vert.glsl SRC)
data_to_c_simple(engines/clay/shaders/clay_particle_vert.glsl SRC)
data_to_c_simple(engines/clay/shaders/clay_particle_strand_frag.glsl SRC)
data_to_c_simple(engines/clay/shaders/ssao_alchemy.glsl SRC)
data_to_c_simple(engines/clay/shaders/ssao_groundtruth.glsl SRC)

data_to_c_simple(engines/eevee/shaders/ambient_occlusion_lib.glsl SRC)
data_to_c_simple(engines/eevee/shaders/default_frag.glsl SRC)
data_to_c_simple(engines/eevee/shaders/default_world_frag.glsl SRC)
data_to_c_simple(engines/eevee/shaders/background_vert.glsl SRC)
data_to_c_simple(engines/eevee/shaders/concentric_samples_lib.glsl SRC)
data_to_c_simple(engines/eevee/shaders/common_uniforms_lib.glsl SRC)
data_to_c_simple(engines/eevee/shaders/lamps_lib.glsl SRC)
data_to_c_simple(engines/eevee/shaders/lightprobe_lib.glsl SRC)
data_to_c_simple(engines/eevee/shaders/lightprobe_filter_glossy_frag.glsl SRC)
data_to_c_simple(engines/eevee/shaders/lightprobe_filter_diffuse_frag.glsl SRC)
data_to_c_simple(engines/eevee/shaders/lightprobe_filter_visibility_frag.glsl SRC)
data_to_c_simple(engines/eevee/shaders/lightprobe_geom.glsl SRC)
data_to_c_simple(engines/eevee/shaders/lightprobe_vert.glsl SRC)
data_to_c_simple(engines/eevee/shaders/lightprobe_cube_display_frag.glsl SRC)
data_to_c_simple(engines/eevee/shaders/lightprobe_cube_display_vert.glsl SRC)
data_to_c_simple(engines/eevee/shaders/lightprobe_grid_display_frag.glsl SRC)
data_to_c_simple(engines/eevee/shaders/lightprobe_grid_display_vert.glsl SRC)
data_to_c_simple(engines/eevee/shaders/lightprobe_grid_fill_frag.glsl SRC)
data_to_c_simple(engines/eevee/shaders/lightprobe_planar_display_frag.glsl SRC)
data_to_c_simple(engines/eevee/shaders/lightprobe_planar_display_vert.glsl SRC)
data_to_c_simple(engines/eevee/shaders/lit_surface_frag.glsl SRC)
data_to_c_simple(engines/eevee/shaders/lit_surface_vert.glsl SRC)
data_to_c_simple(engines/eevee/shaders/effect_bloom_frag.glsl SRC)
data_to_c_simple(engines/eevee/shaders/effect_dof_vert.glsl SRC)
data_to_c_simple(engines/eevee/shaders/effect_dof_frag.glsl SRC)
data_to_c_simple(engines/eevee/shaders/effect_downsample_frag.glsl SRC)
data_to_c_simple(engines/eevee/shaders/effect_downsample_cube_frag.glsl SRC)
data_to_c_simple(engines/eevee/shaders/effect_gtao_frag.glsl SRC)
data_to_c_simple(engines/eevee/shaders/effect_velocity_resolve_frag.glsl SRC)
data_to_c_simple(engines/eevee/shaders/effect_minmaxz_frag.glsl SRC)
data_to_c_simple(engines/eevee/shaders/effect_mist_frag.glsl SRC)
data_to_c_simple(engines/eevee/shaders/effect_motion_blur_frag.glsl SRC)
data_to_c_simple(engines/eevee/shaders/effect_ssr_frag.glsl SRC)
data_to_c_simple(engines/eevee/shaders/effect_subsurface_frag.glsl SRC)
data_to_c_simple(engines/eevee/shaders/effect_temporal_aa.glsl SRC)
data_to_c_simple(engines/eevee/shaders/lightprobe_planar_downsample_frag.glsl SRC)
data_to_c_simple(engines/eevee/shaders/lightprobe_planar_downsample_geom.glsl SRC)
data_to_c_simple(engines/eevee/shaders/lightprobe_planar_downsample_vert.glsl SRC)
data_to_c_simple(engines/eevee/shaders/prepass_frag.glsl SRC)
data_to_c_simple(engines/eevee/shaders/prepass_vert.glsl SRC)
data_to_c_simple(engines/eevee/shaders/shadow_frag.glsl SRC)
data_to_c_simple(engines/eevee/shaders/shadow_vert.glsl SRC)
data_to_c_simple(engines/eevee/shaders/shadow_store_frag.glsl SRC)
data_to_c_simple(engines/eevee/shaders/shadow_copy_frag.glsl SRC)
data_to_c_simple(engines/eevee/shaders/bsdf_lut_frag.glsl SRC)
data_to_c_simple(engines/eevee/shaders/btdf_lut_frag.glsl SRC)
data_to_c_simple(engines/eevee/shaders/bsdf_direct_lib.glsl SRC)
data_to_c_simple(engines/eevee/shaders/bsdf_common_lib.glsl SRC)
data_to_c_simple(engines/eevee/shaders/irradiance_lib.glsl SRC)
data_to_c_simple(engines/eevee/shaders/octahedron_lib.glsl SRC)
data_to_c_simple(engines/eevee/shaders/bsdf_sampling_lib.glsl SRC)
data_to_c_simple(engines/eevee/shaders/raytrace_lib.glsl SRC)
data_to_c_simple(engines/eevee/shaders/ltc_lib.glsl SRC)
data_to_c_simple(engines/eevee/shaders/ssr_lib.glsl SRC)
data_to_c_simple(engines/eevee/shaders/update_noise_frag.glsl SRC)
data_to_c_simple(engines/eevee/shaders/volumetric_lib.glsl SRC)
data_to_c_simple(engines/eevee/shaders/volumetric_frag.glsl SRC)
data_to_c_simple(engines/eevee/shaders/volumetric_geom.glsl SRC)
data_to_c_simple(engines/eevee/shaders/volumetric_vert.glsl SRC)
data_to_c_simple(engines/eevee/shaders/volumetric_resolve_frag.glsl SRC)
data_to_c_simple(engines/eevee/shaders/volumetric_scatter_frag.glsl SRC)
data_to_c_simple(engines/eevee/shaders/volumetric_integration_frag.glsl SRC)

data_to_c_simple(engines/workbench/shaders/workbench_prepass_vert.glsl SRC)
data_to_c_simple(engines/workbench/shaders/workbench_prepass_frag.glsl SRC)
data_to_c_simple(engines/workbench/shaders/workbench_composite_frag.glsl SRC)
data_to_c_simple(engines/workbench/shaders/workbench_shadow_vert.glsl SRC)
data_to_c_simple(engines/workbench/shaders/workbench_shadow_geom.glsl SRC)
data_to_c_simple(engines/workbench/shaders/workbench_background_lib.glsl SRC)
data_to_c_simple(engines/workbench/shaders/workbench_common_lib.glsl SRC)
data_to_c_simple(engines/workbench/shaders/workbench_data_lib.glsl SRC)
data_to_c_simple(engines/workbench/shaders/workbench_object_overlap_lib.glsl SRC)
data_to_c_simple(engines/workbench/shaders/workbench_world_light_lib.glsl SRC)

data_to_c_simple(modes/shaders/common_globals_lib.glsl SRC)
data_to_c_simple(modes/shaders/common_view_lib.glsl SRC)
data_to_c_simple(modes/shaders/common_fxaa_lib.glsl SRC)
data_to_c_simple(modes/shaders/common_fullscreen_vert.glsl SRC)
data_to_c_simple(modes/shaders/armature_axes_vert.glsl SRC)
data_to_c_simple(modes/shaders/armature_sphere_solid_vert.glsl SRC)
data_to_c_simple(modes/shaders/armature_sphere_solid_frag.glsl SRC)
data_to_c_simple(modes/shaders/armature_sphere_outline_vert.glsl SRC)
data_to_c_simple(modes/shaders/armature_envelope_solid_vert.glsl SRC)
data_to_c_simple(modes/shaders/armature_envelope_solid_frag.glsl SRC)
data_to_c_simple(modes/shaders/armature_envelope_outline_vert.glsl SRC)
data_to_c_simple(modes/shaders/armature_envelope_distance_frag.glsl SRC)
data_to_c_simple(modes/shaders/armature_shape_solid_vert.glsl SRC)
data_to_c_simple(modes/shaders/armature_shape_solid_frag.glsl SRC)
data_to_c_simple(modes/shaders/armature_shape_outline_vert.glsl SRC)
data_to_c_simple(modes/shaders/armature_shape_outline_geom.glsl SRC)
data_to_c_simple(modes/shaders/armature_stick_vert.glsl SRC)
data_to_c_simple(modes/shaders/armature_stick_frag.glsl SRC)
data_to_c_simple(modes/shaders/edit_mesh_overlay_frag.glsl SRC)
data_to_c_simple(modes/shaders/edit_mesh_overlay_vert.glsl SRC)
data_to_c_simple(modes/shaders/edit_mesh_overlay_geom_tri.glsl SRC)
data_to_c_simple(modes/shaders/edit_mesh_overlay_geom_edge.glsl SRC)
data_to_c_simple(modes/shaders/edit_mesh_overlay_loosevert_vert.glsl SRC)
data_to_c_simple(modes/shaders/edit_mesh_overlay_facedot_frag.glsl SRC)
data_to_c_simple(modes/shaders/edit_mesh_overlay_facedot_vert.glsl SRC)
data_to_c_simple(modes/shaders/edit_mesh_overlay_mix_frag.glsl SRC)
data_to_c_simple(modes/shaders/edit_mesh_overlay_facefill_vert.glsl SRC)
data_to_c_simple(modes/shaders/edit_mesh_overlay_facefill_frag.glsl SRC)
data_to_c_simple(modes/shaders/edit_curve_overlay_frag.glsl SRC)
data_to_c_simple(modes/shaders/edit_curve_overlay_handle_geom.glsl SRC)
data_to_c_simple(modes/shaders/edit_curve_overlay_loosevert_vert.glsl SRC)
data_to_c_simple(modes/shaders/edit_lattice_overlay_frag.glsl SRC)
data_to_c_simple(modes/shaders/edit_lattice_overlay_loosevert_vert.glsl SRC)
data_to_c_simple(modes/shaders/edit_normals_vert.glsl SRC)
data_to_c_simple(modes/shaders/edit_normals_geom.glsl SRC)
data_to_c_simple(modes/shaders/overlay_face_orientation_frag.glsl SRC)
data_to_c_simple(modes/shaders/overlay_face_orientation_vert.glsl SRC)
data_to_c_simple(modes/shaders/object_empty_image_frag.glsl SRC)
data_to_c_simple(modes/shaders/object_empty_image_vert.glsl SRC)
data_to_c_simple(modes/shaders/object_outline_resolve_frag.glsl SRC)
data_to_c_simple(modes/shaders/object_outline_expand_frag.glsl SRC)
data_to_c_simple(modes/shaders/object_outline_detect_frag.glsl SRC)
data_to_c_simple(modes/shaders/object_outline_prepass_frag.glsl SRC)
data_to_c_simple(modes/shaders/object_grid_frag.glsl SRC)
data_to_c_simple(modes/shaders/object_grid_vert.glsl SRC)
data_to_c_simple(modes/shaders/object_lightprobe_grid_vert.glsl SRC)
data_to_c_simple(modes/shaders/object_mball_handles_vert.glsl SRC)
data_to_c_simple(modes/shaders/object_particle_prim_vert.glsl SRC)
data_to_c_simple(modes/shaders/object_particle_dot_vert.glsl SRC)
data_to_c_simple(modes/shaders/object_particle_dot_frag.glsl SRC)
data_to_c_simple(modes/shaders/paint_texture_frag.glsl SRC)
data_to_c_simple(modes/shaders/paint_texture_vert.glsl SRC)
data_to_c_simple(modes/shaders/paint_vertex_frag.glsl SRC)
data_to_c_simple(modes/shaders/paint_vertex_vert.glsl SRC)
data_to_c_simple(modes/shaders/paint_wire_frag.glsl SRC)
data_to_c_simple(modes/shaders/paint_wire_vert.glsl SRC)
data_to_c_simple(modes/shaders/paint_vert_frag.glsl SRC)
data_to_c_simple(modes/shaders/particle_strand_frag.glsl SRC)
data_to_c_simple(modes/shaders/particle_strand_vert.glsl SRC)










add_definitions(-DKORE_OPENGL)
add_definitions(-DGLEW_STATIC)
add_definitions(-DKORE_G1)
add_definitions(-DKORE_G2)
# add_definitions(-DKORE_G3)
add_definitions(-DKORE_G4)
add_definitions(-DKORE_A1)
add_definitions(-DKORE_A2)
add_definitions(-DKORE_A3)

list(APPEND INC
	./engines/armory/V8/include
	./engines/armory/Kore/Sources
	./engines/armory/Kore/Backends/Graphics4/OpenGL/Sources
	./engines/armory/Kore/Backends/Audio3/A3onA2/Sources
)

list(APPEND SRC
	engines/armory/armory_engine.c
	engines/armory/armory_engine.h

	engines/armory/V8/include/libplatform/libplatform-export.h
	engines/armory/V8/include/libplatform/libplatform.h
	engines/armory/V8/include/libplatform/v8-tracing.h
	engines/armory/V8/include/v8-debug.h
	engines/armory/V8/include/v8-inspector-protocol.h
	engines/armory/V8/include/v8-inspector.h
	engines/armory/V8/include/v8-platform.h
	engines/armory/V8/include/v8-profiler.h
	engines/armory/V8/include/v8-testing.h
	engines/armory/V8/include/v8-util.h
	engines/armory/V8/include/v8-version.h
	engines/armory/V8/include/v8-version-string.h
	engines/armory/V8/include/v8.h
	engines/armory/V8/include/v8config.h

	engines/armory/Kore/Backends/Graphics4/OpenGL/Sources/GL/eglew.h
	engines/armory/Kore/Backends/Graphics4/OpenGL/Sources/GL/glew.c
	engines/armory/Kore/Backends/Graphics4/OpenGL/Sources/GL/glew.h
	engines/armory/Kore/Backends/Graphics4/OpenGL/Sources/GL/glxew.h
	engines/armory/Kore/Backends/Graphics4/OpenGL/Sources/GL/wglew.h
	engines/armory/Kore/Backends/Graphics4/OpenGL/Sources/Kore/ComputeImpl.cpp
	engines/armory/Kore/Backends/Graphics4/OpenGL/Sources/Kore/ComputeImpl.h
	engines/armory/Kore/Backends/Graphics4/OpenGL/Sources/Kore/GraphicsImpl.h
	engines/armory/Kore/Backends/Graphics4/OpenGL/Sources/Kore/IndexBufferImpl.cpp
	engines/armory/Kore/Backends/Graphics4/OpenGL/Sources/Kore/IndexBufferImpl.h
	engines/armory/Kore/Backends/Graphics4/OpenGL/Sources/Kore/ogl.h
	engines/armory/Kore/Backends/Graphics4/OpenGL/Sources/Kore/OpenGL.cpp
	engines/armory/Kore/Backends/Graphics4/OpenGL/Sources/Kore/OpenGL.h
	engines/armory/Kore/Backends/Graphics4/OpenGL/Sources/Kore/pch.h
	engines/armory/Kore/Backends/Graphics4/OpenGL/Sources/Kore/PipelineStateImpl.cpp
	engines/armory/Kore/Backends/Graphics4/OpenGL/Sources/Kore/PipelineStateImpl.h
	engines/armory/Kore/Backends/Graphics4/OpenGL/Sources/Kore/RenderTargetImpl.cpp
	engines/armory/Kore/Backends/Graphics4/OpenGL/Sources/Kore/RenderTargetImpl.h
	engines/armory/Kore/Backends/Graphics4/OpenGL/Sources/Kore/ShaderImpl.cpp
	engines/armory/Kore/Backends/Graphics4/OpenGL/Sources/Kore/ShaderImpl.h
	engines/armory/Kore/Backends/Graphics4/OpenGL/Sources/Kore/ShaderStorageBufferImpl.cpp
	engines/armory/Kore/Backends/Graphics4/OpenGL/Sources/Kore/ShaderStorageBufferImpl.h
	engines/armory/Kore/Backends/Graphics4/OpenGL/Sources/Kore/TextureArrayImpl.cpp
	engines/armory/Kore/Backends/Graphics4/OpenGL/Sources/Kore/TextureArrayImpl.h
	engines/armory/Kore/Backends/Graphics4/OpenGL/Sources/Kore/TextureImpl.cpp
	engines/armory/Kore/Backends/Graphics4/OpenGL/Sources/Kore/TextureImpl.h
	engines/armory/Kore/Backends/Graphics4/OpenGL/Sources/Kore/VertexBufferImpl.cpp
	engines/armory/Kore/Backends/Graphics4/OpenGL/Sources/Kore/VertexBufferImpl.h
	engines/armory/Kore/Backends/Graphics4/OpenGL/Sources/Kore/VrInterface.cpp

	engines/armory/Kore/Sources/Kore/Audio1/Audio.cpp
	engines/armory/Kore/Sources/Kore/Audio1/Audio.h
	engines/armory/Kore/Sources/Kore/Audio1/pch.h
	engines/armory/Kore/Sources/Kore/Audio1/Sound.cpp
	engines/armory/Kore/Sources/Kore/Audio1/Sound.h
	engines/armory/Kore/Sources/Kore/Audio1/SoundStream.cpp
	engines/armory/Kore/Sources/Kore/Audio1/SoundStream.h
	engines/armory/Kore/Sources/Kore/Audio1/stb_vorbis.c
	engines/armory/Kore/Sources/Kore/Audio2/Audio.cpp
	engines/armory/Kore/Sources/Kore/Audio2/Audio.h
	engines/armory/Kore/Sources/Kore/Audio2/pch.h
	engines/armory/Kore/Sources/Kore/Audio3/Audio.cpp
	engines/armory/Kore/Sources/Kore/Audio3/Audio.h
	engines/armory/Kore/Sources/Kore/Audio3/pch.h
	engines/armory/Kore/Sources/Kore/CodeStyle.cpp
	engines/armory/Kore/Sources/Kore/Compute/Compute.h
	engines/armory/Kore/Sources/Kore/Error.cpp
	engines/armory/Kore/Sources/Kore/Error.h
	engines/armory/Kore/Sources/Kore/ErrorArgs.h
	engines/armory/Kore/Sources/Kore/Graphics1/Color.cpp
	engines/armory/Kore/Sources/Kore/Graphics1/Color.h
	engines/armory/Kore/Sources/Kore/Graphics1/Image.cpp
	engines/armory/Kore/Sources/Kore/Graphics1/Image.h
	engines/armory/Kore/Sources/Kore/Graphics1/pch.h
	engines/armory/Kore/Sources/Kore/Graphics1/stb_image.h
	engines/armory/Kore/Sources/Kore/Graphics2/Graphics.cpp
	engines/armory/Kore/Sources/Kore/Graphics2/Graphics.h
	engines/armory/Kore/Sources/Kore/Graphics2/Kravur.cpp
	engines/armory/Kore/Sources/Kore/Graphics2/Kravur.h
	engines/armory/Kore/Sources/Kore/Graphics2/pch.h
	# engines/armory/Kore/Sources/Kore/Graphics3/Graphics.cpp
	# engines/armory/Kore/Sources/Kore/Graphics3/Graphics.h
	# engines/armory/Kore/Sources/Kore/Graphics3/Light.h
	# engines/armory/Kore/Sources/Kore/Graphics3/pch.h
	# engines/armory/Kore/Sources/Kore/Graphics3/Texture.cpp
	# engines/armory/Kore/Sources/Kore/Graphics3/Texture.h
	engines/armory/Kore/Sources/Kore/Graphics4/Graphics.cpp
	engines/armory/Kore/Sources/Kore/Graphics4/Graphics.h
	engines/armory/Kore/Sources/Kore/Graphics4/pch.h
	engines/armory/Kore/Sources/Kore/Graphics4/PipelineState.cpp
	engines/armory/Kore/Sources/Kore/Graphics4/PipelineState.h
	engines/armory/Kore/Sources/Kore/Graphics4/Shader.h
	engines/armory/Kore/Sources/Kore/Graphics4/Texture.cpp
	engines/armory/Kore/Sources/Kore/Graphics4/Texture.h
	engines/armory/Kore/Sources/Kore/Graphics4/TextureArray.cpp
	engines/armory/Kore/Sources/Kore/Graphics4/TextureArray.h
	engines/armory/Kore/Sources/Kore/Graphics4/VertexStructure.h
	engines/armory/Kore/Sources/Kore/Input/Gamepad.cpp
	engines/armory/Kore/Sources/Kore/Input/Gamepad.h
	engines/armory/Kore/Sources/Kore/Input/Keyboard.cpp
	engines/armory/Kore/Sources/Kore/Input/Keyboard.h
	engines/armory/Kore/Sources/Kore/Input/Mouse.cpp
	engines/armory/Kore/Sources/Kore/Input/Mouse.h
	engines/armory/Kore/Sources/Kore/Input/pch.h
	engines/armory/Kore/Sources/Kore/Input/Pen.cpp
	engines/armory/Kore/Sources/Kore/Input/Pen.h
	engines/armory/Kore/Sources/Kore/Input/Sensor.cpp
	engines/armory/Kore/Sources/Kore/Input/Sensor.h
	engines/armory/Kore/Sources/Kore/Input/Surface.cpp
	engines/armory/Kore/Sources/Kore/Input/Surface.h
	engines/armory/Kore/Sources/Kore/IO/BufferReader.cpp
	engines/armory/Kore/Sources/Kore/IO/BufferReader.h
	engines/armory/Kore/Sources/Kore/IO/FileReader.h
	engines/armory/Kore/Sources/Kore/IO/FileReader.winrt.cpp
	engines/armory/Kore/Sources/Kore/IO/FileWriter.cpp
	engines/armory/Kore/Sources/Kore/IO/FileWriter.h
	engines/armory/Kore/Sources/Kore/IO/lz4/LICENSE
	engines/armory/Kore/Sources/Kore/IO/lz4/lz4.c
	engines/armory/Kore/Sources/Kore/IO/lz4/lz4.h
	engines/armory/Kore/Sources/Kore/IO/lz4/lz4frame.c
	engines/armory/Kore/Sources/Kore/IO/lz4/lz4frame.h
	engines/armory/Kore/Sources/Kore/IO/lz4/lz4frame_static.h
	engines/armory/Kore/Sources/Kore/IO/lz4/lz4hc.c
	engines/armory/Kore/Sources/Kore/IO/lz4/lz4hc.h
	engines/armory/Kore/Sources/Kore/IO/lz4/lz4opt.h
	engines/armory/Kore/Sources/Kore/IO/lz4/xxhash.c
	engines/armory/Kore/Sources/Kore/IO/lz4/xxhash.h
	engines/armory/Kore/Sources/Kore/IO/pch.h
	engines/armory/Kore/Sources/Kore/IO/Reader.cpp
	engines/armory/Kore/Sources/Kore/IO/Reader.h
	engines/armory/Kore/Sources/Kore/IO/Writer.cpp
	engines/armory/Kore/Sources/Kore/IO/Writer.h
	engines/armory/Kore/Sources/Kore/Log.cpp
	engines/armory/Kore/Sources/Kore/Log.h
	engines/armory/Kore/Sources/Kore/LogArgs.h
	engines/armory/Kore/Sources/Kore/Math/Core.cpp
	engines/armory/Kore/Sources/Kore/Math/Core.h
	engines/armory/Kore/Sources/Kore/Math/Matrix.h
	engines/armory/Kore/Sources/Kore/Math/pch.h
	engines/armory/Kore/Sources/Kore/Math/Quaternion.cpp
	engines/armory/Kore/Sources/Kore/Math/Quaternion.h
	engines/armory/Kore/Sources/Kore/Math/Random.cpp
	engines/armory/Kore/Sources/Kore/Math/Random.h
	engines/armory/Kore/Sources/Kore/Math/Vector.h
	engines/armory/Kore/Sources/Kore/Network/Connection.cpp
	engines/armory/Kore/Sources/Kore/Network/Connection.h
	engines/armory/Kore/Sources/Kore/Network/Http.cpp
	engines/armory/Kore/Sources/Kore/Network/Http.h
	engines/armory/Kore/Sources/Kore/Network/pch.h
	engines/armory/Kore/Sources/Kore/Network/Socket.cpp
	engines/armory/Kore/Sources/Kore/Network/Socket.h
	engines/armory/Kore/Sources/Kore/pch.h
	engines/armory/Kore/Sources/Kore/Simd/float32x4.h
	engines/armory/Kore/Sources/Kore/System.cpp
	engines/armory/Kore/Sources/Kore/System.h
	engines/armory/Kore/Sources/Kore/Threads/Mutex.h
	engines/armory/Kore/Sources/Kore/Threads/Thread.h
	engines/armory/Kore/Sources/Kore/Window.h
	
	engines/armory/Kore/Backends/Audio3/A3onA2/Sources/Kore/Audio.cpp
	engines/armory/Kore/Backends/Audio3/A3onA2/Sources/Kore/pch.h
)


if(WIN32)

list(APPEND INC
	./engines/armory/Kore/Backends/System/Windows/Sources
	./engines/armory/Kore/Backends/System/Microsoft/Sources
	./engines/armory/Kore/Backends/Audio2/DirectSound/Sources
	./engines/armory/Kore/Backends/System/Windows/Libraries/DirectShow/BaseClasses
)

list(APPEND SRC
	engines/armory/Kore/Backends/System/Microsoft/Sources/Kore/Event.cpp
	engines/armory/Kore/Backends/System/Microsoft/Sources/Kore/EventImpl.h
	engines/armory/Kore/Backends/System/Microsoft/Sources/Kore/Mutex.cpp
	engines/armory/Kore/Backends/System/Microsoft/Sources/Kore/MutexImpl.h
	engines/armory/Kore/Backends/System/Microsoft/Sources/Kore/pch.h
	engines/armory/Kore/Backends/System/Microsoft/Sources/Kore/Semaphore.cpp
	engines/armory/Kore/Backends/System/Microsoft/Sources/Kore/SemaphoreImpl.h
	engines/armory/Kore/Backends/System/Microsoft/Sources/Kore/SystemMicrosoft.cpp
	engines/armory/Kore/Backends/System/Microsoft/Sources/Kore/SystemMicrosoft.h
	engines/armory/Kore/Backends/System/Microsoft/Sources/Kore/Thread.cpp
	engines/armory/Kore/Backends/System/Microsoft/Sources/Kore/ThreadImpl.h
	engines/armory/Kore/Backends/System/Microsoft/Sources/Kore/ThreadLocal.cpp
	engines/armory/Kore/Backends/System/Microsoft/Sources/Kore/ThreadLocalImpl.h

	engines/armory/Kore/Backends/System/Windows/Sources/Kore/Display.cpp
	engines/armory/Kore/Backends/System/Windows/Sources/Kore/Display.h
	engines/armory/Kore/Backends/System/Windows/Sources/Kore/Input/Mouse.cpp
	engines/armory/Kore/Backends/System/Windows/Sources/Kore/pch.h
	engines/armory/Kore/Backends/System/Windows/Sources/Kore/System.cpp
	engines/armory/Kore/Backends/System/Windows/Sources/Kore/SystemWindows.cpp
	engines/armory/Kore/Backends/System/Windows/Sources/Kore/SystemWindows.h
	engines/armory/Kore/Backends/System/Windows/Sources/Kore/Video.cpp
	engines/armory/Kore/Backends/System/Windows/Sources/Kore/Video.h
	engines/armory/Kore/Backends/System/Windows/Sources/Kore/VideoSoundStream.h
	engines/armory/Kore/Backends/System/Windows/Sources/Kore/VrInterface_SteamVR.cpp

	engines/armory/Kore/Backends/Audio2/DirectSound/Sources/Kore/DirectSound.cpp
	engines/armory/Kore/Backends/Audio2/DirectSound/Sources/Kore/pch.h
	
	engines/armory/Kore/Backends/System/Windows/Libraries/DirectShow/BaseClasses/amextra.cpp
	engines/armory/Kore/Backends/System/Windows/Libraries/DirectShow/BaseClasses/amextra.h
	engines/armory/Kore/Backends/System/Windows/Libraries/DirectShow/BaseClasses/amfilter.cpp
	engines/armory/Kore/Backends/System/Windows/Libraries/DirectShow/BaseClasses/amfilter.h
	engines/armory/Kore/Backends/System/Windows/Libraries/DirectShow/BaseClasses/amvideo.cpp
	engines/armory/Kore/Backends/System/Windows/Libraries/DirectShow/BaseClasses/arithutil.cpp
	engines/armory/Kore/Backends/System/Windows/Libraries/DirectShow/BaseClasses/cache.h
	engines/armory/Kore/Backends/System/Windows/Libraries/DirectShow/BaseClasses/checkbmi.h
	engines/armory/Kore/Backends/System/Windows/Libraries/DirectShow/BaseClasses/combase.cpp
	engines/armory/Kore/Backends/System/Windows/Libraries/DirectShow/BaseClasses/combase.h
	engines/armory/Kore/Backends/System/Windows/Libraries/DirectShow/BaseClasses/cprop.cpp
	engines/armory/Kore/Backends/System/Windows/Libraries/DirectShow/BaseClasses/cprop.h
	engines/armory/Kore/Backends/System/Windows/Libraries/DirectShow/BaseClasses/ctlutil.cpp
	engines/armory/Kore/Backends/System/Windows/Libraries/DirectShow/BaseClasses/ctlutil.h
	engines/armory/Kore/Backends/System/Windows/Libraries/DirectShow/BaseClasses/ddmm.cpp
	engines/armory/Kore/Backends/System/Windows/Libraries/DirectShow/BaseClasses/ddmm.h
	engines/armory/Kore/Backends/System/Windows/Libraries/DirectShow/BaseClasses/dllentry.cpp
	engines/armory/Kore/Backends/System/Windows/Libraries/DirectShow/BaseClasses/dllsetup.cpp
	engines/armory/Kore/Backends/System/Windows/Libraries/DirectShow/BaseClasses/dllsetup.h
	engines/armory/Kore/Backends/System/Windows/Libraries/DirectShow/BaseClasses/dxmperf.h
	engines/armory/Kore/Backends/System/Windows/Libraries/DirectShow/BaseClasses/fourcc.h
	engines/armory/Kore/Backends/System/Windows/Libraries/DirectShow/BaseClasses/measure.h
	engines/armory/Kore/Backends/System/Windows/Libraries/DirectShow/BaseClasses/msgthrd.h
	engines/armory/Kore/Backends/System/Windows/Libraries/DirectShow/BaseClasses/mtype.cpp
	engines/armory/Kore/Backends/System/Windows/Libraries/DirectShow/BaseClasses/mtype.h
	engines/armory/Kore/Backends/System/Windows/Libraries/DirectShow/BaseClasses/outputq.cpp
	engines/armory/Kore/Backends/System/Windows/Libraries/DirectShow/BaseClasses/outputq.h
	engines/armory/Kore/Backends/System/Windows/Libraries/DirectShow/BaseClasses/perflog.cpp
	engines/armory/Kore/Backends/System/Windows/Libraries/DirectShow/BaseClasses/perflog.h
	engines/armory/Kore/Backends/System/Windows/Libraries/DirectShow/BaseClasses/perfstruct.h
	engines/armory/Kore/Backends/System/Windows/Libraries/DirectShow/BaseClasses/pstream.cpp
	engines/armory/Kore/Backends/System/Windows/Libraries/DirectShow/BaseClasses/pstream.h
	engines/armory/Kore/Backends/System/Windows/Libraries/DirectShow/BaseClasses/pullpin.cpp
	engines/armory/Kore/Backends/System/Windows/Libraries/DirectShow/BaseClasses/pullpin.h
	engines/armory/Kore/Backends/System/Windows/Libraries/DirectShow/BaseClasses/refclock.cpp
	engines/armory/Kore/Backends/System/Windows/Libraries/DirectShow/BaseClasses/refclock.h
	engines/armory/Kore/Backends/System/Windows/Libraries/DirectShow/BaseClasses/reftime.h
	engines/armory/Kore/Backends/System/Windows/Libraries/DirectShow/BaseClasses/renbase.cpp
	engines/armory/Kore/Backends/System/Windows/Libraries/DirectShow/BaseClasses/renbase.h
	engines/armory/Kore/Backends/System/Windows/Libraries/DirectShow/BaseClasses/schedule.cpp
	engines/armory/Kore/Backends/System/Windows/Libraries/DirectShow/BaseClasses/schedule.h
	engines/armory/Kore/Backends/System/Windows/Libraries/DirectShow/BaseClasses/seekpt.cpp
	engines/armory/Kore/Backends/System/Windows/Libraries/DirectShow/BaseClasses/seekpt.h
	engines/armory/Kore/Backends/System/Windows/Libraries/DirectShow/BaseClasses/source.cpp
	engines/armory/Kore/Backends/System/Windows/Libraries/DirectShow/BaseClasses/source.h
	engines/armory/Kore/Backends/System/Windows/Libraries/DirectShow/BaseClasses/streams.h
	engines/armory/Kore/Backends/System/Windows/Libraries/DirectShow/BaseClasses/strmctl.cpp
	engines/armory/Kore/Backends/System/Windows/Libraries/DirectShow/BaseClasses/strmctl.h
	engines/armory/Kore/Backends/System/Windows/Libraries/DirectShow/BaseClasses/sysclock.cpp
	engines/armory/Kore/Backends/System/Windows/Libraries/DirectShow/BaseClasses/sysclock.h
	engines/armory/Kore/Backends/System/Windows/Libraries/DirectShow/BaseClasses/transfrm.cpp
	engines/armory/Kore/Backends/System/Windows/Libraries/DirectShow/BaseClasses/transfrm.h
	engines/armory/Kore/Backends/System/Windows/Libraries/DirectShow/BaseClasses/transip.cpp
	engines/armory/Kore/Backends/System/Windows/Libraries/DirectShow/BaseClasses/transip.h
	engines/armory/Kore/Backends/System/Windows/Libraries/DirectShow/BaseClasses/videoctl.cpp
	engines/armory/Kore/Backends/System/Windows/Libraries/DirectShow/BaseClasses/videoctl.h
	engines/armory/Kore/Backends/System/Windows/Libraries/DirectShow/BaseClasses/vtrans.cpp
	engines/armory/Kore/Backends/System/Windows/Libraries/DirectShow/BaseClasses/vtrans.h
	engines/armory/Kore/Backends/System/Windows/Libraries/DirectShow/BaseClasses/winctrl.cpp
	engines/armory/Kore/Backends/System/Windows/Libraries/DirectShow/BaseClasses/winctrl.h
	engines/armory/Kore/Backends/System/Windows/Libraries/DirectShow/BaseClasses/winutil.cpp
	engines/armory/Kore/Backends/System/Windows/Libraries/DirectShow/BaseClasses/winutil.h
	engines/armory/Kore/Backends/System/Windows/Libraries/DirectShow/BaseClasses/wxdebug.cpp
	engines/armory/Kore/Backends/System/Windows/Libraries/DirectShow/BaseClasses/wxdebug.h
	engines/armory/Kore/Backends/System/Windows/Libraries/DirectShow/BaseClasses/wxlist.cpp
	engines/armory/Kore/Backends/System/Windows/Libraries/DirectShow/BaseClasses/wxlist.h
	engines/armory/Kore/Backends/System/Windows/Libraries/DirectShow/BaseClasses/wxutil.cpp
	engines/armory/Kore/Backends/System/Windows/Libraries/DirectShow/BaseClasses/wxutil.h
)

add_definitions(-DKORE_WINDOWS)
add_definitions(-DKORE_MICROSOFT)
add_definitions(-D_CRT_SECURE_NO_WARNINGS)
add_definitions(-D_WINSOCK_DEPRECATED_NO_WARNINGS)


endif()


if(APPLE)
endif()


if(UNIX AND NOT APPLE)
endif()


blender_add_lib(bf_draw "${SRC}" "${INC}" "${INC_SYS}")


if(WIN32)
target_link_libraries(bf_draw dxguid)
target_link_libraries(bf_draw dsound)
target_link_libraries(bf_draw dinput8)
target_link_libraries(bf_draw ws2_32)
target_link_libraries(bf_draw strmiids)
target_link_libraries(bf_draw winmm)
target_link_libraries(bf_draw C:/Users/lubos/Downloads/blender-git/blender/source/blender/draw/engines/armory/V8/Libraries/win32/release/v8.dll.lib)
target_link_libraries(bf_draw C:/Users/lubos/Downloads/blender-git/blender/source/blender/draw/engines/armory/V8/Libraries/win32/release/v8_libplatform.dll.lib)
target_link_libraries(bf_draw C:/Users/lubos/Downloads/blender-git/blender/source/blender/draw/engines/armory/V8/Libraries/win32/release/v8_libbase.dll.lib)
endif()

if(APPLE)
endif()

if(UNIX AND NOT APPLE)
endif()
