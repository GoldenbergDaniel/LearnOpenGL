#pragma once

#include <assert.h>
#include "glad/glad.h"

#include "common.hpp"
#include "util.hpp"

struct GFX_Shader
{
  u32 id;
};

struct GFX_Object
{
  u32 id;
  u32 attrib_count;
  u32 attrib_index;
};

struct GFX_Layout
{
  u32 index;
  u32 count;
  GLenum data_type;
  bool normalized;
  u32 stride;
  void *first;
};

#define DEBUG

#ifdef DEBUG
#define GFX_ASSERT(call) \
  _gfx_clear_error(); \
  call; \
  assert(_gfx_check_error())
#else
#define GFX_ASSERT(call) \
  call;
#endif

bool _gfx_check_error();
void _gfx_clear_error();

GFX_Shader gfx_create_shader(String *vert_src, String *frag_src);
void gfx_bind_shader(GFX_Shader *shader);
void gfx_unbind_shader();
i32 gfx_set_shader_uniform(GFX_Shader *shader, String *name, u32 val);
i32 gfx_set_shader_uniform(GFX_Shader *shader, String *name, i32 val);
i32 gfx_set_shader_uniform(GFX_Shader *shader, String *name, f32 val);
i32 gfx_set_shader_uniform(GFX_Shader *shader, String *name, Vec2F val);
i32 gfx_set_shader_uniform(GFX_Shader *shader, String *name, Vec3F val);
i32 gfx_set_shader_uniform(GFX_Shader *shader, String *name, Vec4F val);

GFX_Object gfx_create_vertex_buffer(void *data, u32 size);
void gfx_bind_vertex_buffer(GFX_Object *vertex_buffer);
void gfx_unbind_vertex_buffer();

GFX_Object gfx_create_index_buffer(void *data, u32 size);
void gfx_bind_index_buffer(GFX_Object *index_buffer);
void gfx_unbind_index_buffer();

GFX_Object gfx_create_vertex_array(u32 attrib_count);
void gfx_bind_vertex_array(GFX_Object *vertex_array);
void gfx_unbind_vertex_array();
GFX_Layout gfx_add_vertex_layout(GFX_Object *vertex_array, GLenum type, u32 count);
void gfx_set_vertex_layout(GFX_Object *vertex_array, GFX_Layout *layout);

void gfx_clear(Vec4F color);
void gfx_draw_rect(Vec2F pos, Vec2F dim, Vec2F color);
