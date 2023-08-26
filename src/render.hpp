#pragma once

#include <assert.h>
#include "glad/glad.h"

#include "common.hpp"
#include "util.hpp"

struct R_Shader
{
  u32 id;
};

struct R_Object
{
  u32 id;
  u32 attrib_count;
  u32 attrib_index;
};

struct R_Layout
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
#define R_ASSERT(call) \
  _r_clear_error(); \
  call; \
  assert(_r_check_error())
#else
#define R_ASSERT(call) \
  call;
#endif

bool _r_check_error();
void _r_clear_error();

R_Shader r_create_shader(i8 *vert_src, i8 *frag_src);
void r_bind_shader(R_Shader *shader);
void r_unbind_shader();
i32 r_set_uniform(R_Shader *shader, i8 *name, u32 val);
i32 r_set_uniform(R_Shader *shader, i8 *name, i32 val);
i32 r_set_uniform(R_Shader *shader, i8 *name, f32 val);
i32 r_set_uniform(R_Shader *shader, i8 *name, Vec2F val);
i32 r_set_uniform(R_Shader *shader, i8 *name, Vec3F val);
i32 r_set_uniform(R_Shader *shader, i8 *name, Vec4F val);
i32 r_set_uniform(R_Shader *shader, i8 *name, Vec4F val);

R_Object r_create_vertex_buffer(void *data, u32 size);
void r_bind_vertex_buffer(R_Object *vertex_buffer);
void r_unbind_vertex_buffer();

R_Object r_create_index_buffer(void *data, u32 size);
void r_bind_index_buffer(R_Object *index_buffer);
void r_unbind_index_buffer();

R_Object r_create_vertex_array(u32 attrib_count);
void r_bind_vertex_array(R_Object *vertex_array);
void r_unbind_vertex_array();
R_Layout r_add_vertex_layout(R_Object *vertex_array, GLenum type, u32 count);
void r_set_vertex_layout(R_Object *vertex_array, R_Layout *layout);

void r_clear(Vec4F color);
void r_draw_rect(Vec2F pos, Vec2F dim, Vec2F color);
