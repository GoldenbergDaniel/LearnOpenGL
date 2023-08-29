#pragma once

#include <assert.h>
#include "glad/glad.h"

#include "common.h"
#include "util.h"

struct R_Vertex
{
  f32 position[3];
  f32 color[3];
};

struct R_Object
{
  u32 id;
  u8 attrib_count;
  u8 attrib_index;
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

struct R_Shader
{
  u32 id;
};

struct R_Texture2D
{
  u32 id;
  i32 width;
  i32 height;
};

#define R_BufferType_V 0
#define R_BufferType_I 1

#define DEBUG

#ifdef DEBUG
#define R_ASSERT(call) \
  _r_clear_error(); \
  call; \
  ASSERT(_r_check_error())
#else
#define R_ASSERT(call) \
  call;
#endif

bool _r_check_error();
void _r_clear_error();

// Shader ----------------------------------------------------------------------

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

// Buffer ----------------------------------------------------------------------

R_Object r_create_buffer(u8 type, void *data, u32 size);
R_Object r_create_buffer(u8 type, R_Vertex *vertex, u32 size);
void r_bind_buffer(u8 type, R_Object *buffer);
void r_unbind_buffer(u8 type);

// Vertex Array ----------------------------------------------------------------

R_Object r_create_vertex_array(u8 attrib_count);
void r_bind_vertex_array(R_Object *vertex_array);
void r_unbind_vertex_array();
R_Layout r_add_vertex_layout(R_Object *vertex_array, GLenum type, u32 count);
void r_set_vertex_layout(R_Object *vertex_array, R_Layout *layout);

// Texture ---------------------------------------------------------------------

R_Texture2D r_create_tex2d();

// Draw ------------------------------------------------------------------------

void d_clear(Vec4F color);
void d_draw_rect(Vec2F pos, Vec2F dim, Vec2F color);
