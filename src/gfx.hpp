#pragma once

#include <assert.h>

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

struct Shader
{
  u32 id;
};

#define GFX_GL_ASSERT(call) \
  _gfx_clear_error(); \
  call; \
  assert(_gfx_check_error())

bool _gfx_check_error();
void _gfx_clear_error();

Shader gfx_create_shader(const i8 *vert_src, const i8 *frag_src);
void gfx_use_shader(Shader *shader);
void gfx_set_shader_uniform(Shader *shader, i8 *name, u32 value);
void gfx_set_shader_uniform(Shader *shader, i8 *name, Vec3F value);
void gfx_set_shader_uniform(Shader *shader, i8 *name, Vec4F value);
void gfx_set_shader_uniform(Shader *shader, i8 *name, i32 value);
void gfx_set_shader_uniform(Shader *shader, i8 *name, f32 value);

void gfx_clear(Vec4F color);
void gfx_draw_rect(Vec2F pos, Vec2F dim, Vec2F color);
