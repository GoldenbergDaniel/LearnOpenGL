#pragma once

#include <assert.h>
#include "glad/glad.h"

#include "common.hpp"

struct Vec2F
{
  union { f32 x; f32 w; };
  union { f32 y; f32 h; };
};

struct Vec3F
{
  f32 x;
  f32 y;
  f32 z;
};

struct Vec4F
{
  union { f32 x; f32 r; };
  union { f32 y; f32 g; };
  union { f32 z; f32 b; };
  union { f32 w; f32 a; };
};

struct GLObject
{
  u32 id;
};

struct GLAttribute
{
  u32 index;
  u32 count;
  GLenum data_type;
  bool normalized;
  u32 stride;
  void *first;
};

#define ATTRIBUTE_COUNT 2

#define GFX_GL_ASSERT(call) \
  _gfx_clear_error(); \
  call; \
  assert(_gfx_check_error())

bool _gfx_check_error();
void _gfx_clear_error();

GLObject gfx_create_shader(const i8 *vert_src, const i8 *frag_src);
void gfx_bind_shader(GLObject *shader);
void gfx_unbind_shader();
// void gfx_set_shader_uniform(Shader *shader, i8 *name, u32 val);
// void gfx_set_shader_uniform(Shader *shader, i8 *name, Vec3F val);
// void gfx_set_shader_uniform(Shader *shader, i8 *name, Vec4F val);
// void gfx_set_shader_uniform(Shader *shader, i8 *name, i32 val);
// void gfx_set_shader_uniform(Shader *shader, i8 *name, f32 val);

GLObject gfx_create_vertex_buffer(void *data, u32 size);
void gfx_bind_vertex_buffer(GLObject *vertex_buffer);
void gfx_unbind_vertex_buffer();

GLObject gfx_create_index_buffer(void *data, u32 size);
void gfx_bind_index_buffer(GLObject *index_buffer);
void gfx_unbind_index_buffer();

GLObject gfx_create_vertex_array();
GLAttribute gfx_create_vertex_attrib(u32 index, u32 count, GLenum data_type);
void gfx_set_vertex_attrib(GLObject *vertex_array, GLAttribute *attrib);
void gfx_bind_vertex_array(GLObject *vertex_array);
void gfx_unbind_vertex_array();

void gfx_clear(Vec4F color);
void gfx_draw_rect(Vec2F pos, Vec2F dim, Vec2F color);
