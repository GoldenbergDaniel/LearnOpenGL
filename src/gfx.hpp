#pragma once

#include <assert.h>
#include "glad/glad.h"

#include "common.hpp"
#include "util.hpp"

#define DEBUG

#ifdef DEBUG
#define GFX_GL_ASSERT(call) \
  gfx::_clear_error(); \
  call; \
  assert(gfx::_check_error())
#else
#define GFX_GL_ASSERT(call) \
  call;
#endif

namespace gfx {

struct GLObject
{
  u32 id;
  u32 attrib_count;
  u32 attrib_index;
};

struct GLLayout
{
  u32 index;
  u32 count;
  GLenum data_type;
  bool normalized;
  u32 stride;
  void *first;
};

bool _check_error();
void _clear_error();

GLObject create_shader(String *vert_src, String *frag_src);
void bind(GLObject *shader);
void unbind_shader();
i32 get_shader_uniform(GLObject *shader, String *name, u32 val);
i32 set_shader_uniform(GLObject *shader, String *name, i32 val);
i32 set_shader_uniform(GLObject *shader, String *name, f32 val);
i32 set_shader_uniform(GLObject *shader, String *name, Vec2F val);
i32 set_shader_uniform(GLObject *shader, String *name, Vec3F val);
i32 set_shader_uniform(GLObject *shader, String *name, Vec4F val);

GLObject create_vertex_buffer(void *data, u32 size);
void bind(GLObject *vertex_buffer);
void unbind_vertex_buffer();

GLObject create_index_buffer(void *data, u32 size);
void bind(GLObject *index_buffer);
void unbind_index_buffer();

GLObject create_vertex_array(u32 attrib_count);
void bind(GLObject *vertex_array);
void unbind_vertex_array();
GLLayout add_vertex_layout(GLObject *vertex_array, u32 count, GLenum type);
void set_vertex_layout(GLObject *vertex_array, GLLayout *layout);

void clear(Vec4F color);

}
