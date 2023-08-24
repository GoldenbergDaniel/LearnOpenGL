#include <stdio.h>
#include <alloca.h>
#include <assert.h>
#include <SDL2/SDL.h>

#include "glad/glad.h"

#include "common.hpp"
#include "util.hpp"
#include "gfx.hpp"

static void gfx_verify_shader(u32 shader, u32 type);

bool _gfx_check_error()
{
  while (u32 err = glGetError())
  {
    printf("[OpenGL Error]: %u\n", err);
    return false;
  }

  return true;
}

void _gfx_clear_error()
{
  while (glGetError() != GL_NO_ERROR);
}

// Shader ----------------------------------------------------------------------

GLObject gfx_create_shader(String *vert_src, String *frag_src)
{
  u32 vert = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vert, 1, &vert_src->data, nullptr);
  glCompileShader(vert);
  gfx_verify_shader(vert, GL_COMPILE_STATUS);

  u32 frag = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(frag, 1, &frag_src->data, nullptr);
  glCompileShader(frag);
  gfx_verify_shader(frag, GL_COMPILE_STATUS);

  u32 id = glCreateProgram();
  glAttachShader(id, frag);
  glAttachShader(id, vert);
  glLinkProgram(id);
  gfx_verify_shader(id, GL_LINK_STATUS);

  glDeleteShader(vert);
  glDeleteShader(frag);

  return (GLObject) {id};
}

void gfx_bind_shader(GLObject *shader)
{
  GFX_GL_ASSERT(glUseProgram(shader->id));
}

void gfx_unbind_shader()
{
  glUseProgram(0);
}

i32 gfx_set_shader_uniform(GLObject *shader, String *name, u32 val)
{
  i32 loc = glGetUniformLocation(shader->id, name->data);
  glUniform1ui(loc, val);
  
  return loc;
}

i32 gfx_set_shader_uniform(GLObject *shader, String *name, i32 val)
{
  i32 loc = glGetUniformLocation(shader->id, name->data);
  glUniform1i(loc, val);
  
  return loc;
}

i32 gfx_set_shader_uniform(GLObject *shader, String *name, f32 val)
{
  i32 loc = glGetUniformLocation(shader->id, name->data);
  glUniform1f(loc, val);
  
  return loc;
}

i32 gfx_set_shader_uniform(GLObject *shader, String *name, Vec2F val)
{
  i32 loc = glGetUniformLocation(shader->id, name->data);
  glUniform2f(loc, val.x, val.y);
  
  return loc;
}

i32 gfx_set_shader_uniform(GLObject *shader, String *name, Vec3F val)
{
  i32 loc = glGetUniformLocation(shader->id, name->data);
  glUniform3f(loc, val.x, val.y, val.z);
  
  return loc;
}

i32 gfx_set_shader_uniform(GLObject *shader, String *name, Vec4F val)
{
  i32 loc = glGetUniformLocation(shader->id, name->data);
  glUniform4f(loc, val.x, val.y, val.z, val.w);
  
  return loc;
}

static
void gfx_verify_shader(u32 shader, u32 type)
{
  i32 success;

  if (type == GL_LINK_STATUS)
  {
    glValidateProgram(shader);
  }

  glGetShaderiv(shader, type, &success);

  if (!success)
  {
    i32 length;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
    i8 *log = (i8 *) alloca(length);
    glGetShaderInfoLog(shader, length, &length, log);

    if (type == GL_COMPILE_STATUS)
    {
      printf("[GLObject Error]: Failed to compile shader!\n");
    }
    else
    {
      printf("[GLObject Error]: Failed to link shaders!\n");
    }

    printf("%s", log);
  }
}

// Vertex Buffer ---------------------------------------------------------------

GLObject gfx_create_vertex_buffer(void *data, u32 size)
{
  u32 id;
  glGenBuffers(1, &id);
  GFX_GL_ASSERT(glBindBuffer(GL_ARRAY_BUFFER, id));
  GFX_GL_ASSERT(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));

  return (GLObject) {id};
}

void gfx_bind_vertex_buffer(GLObject *vertex_buffer)
{
  GFX_GL_ASSERT(glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer->id));
}

void gfx_unbind_vertex_buffer()
{
  GFX_GL_ASSERT(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

// Index Buffer ----------------------------------------------------------------

GLObject gfx_create_index_buffer(void *data, u32 size)
{
  u32 id;
  glGenBuffers(1, &id);
  GFX_GL_ASSERT(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id));
  GFX_GL_ASSERT(glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));

  return (GLObject) {id};
}

void gfx_bind_index_buffer(GLObject *vertex_buffer)
{
  GFX_GL_ASSERT(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertex_buffer->id));
}

void gfx_unbind_index_buffer()
{
  GFX_GL_ASSERT(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

// Vertex Array ----------------------------------------------------------------

GLObject gfx_create_vertex_array(u32 attrib_count)
{
  u32 id;
  glGenVertexArrays(1, &id);

  return (GLObject) {id, attrib_count, 0};
}

void gfx_bind_vertex_array(GLObject *array)
{
  GFX_GL_ASSERT(glBindVertexArray(array->id));
}

void gfx_unbind_vertex_array()
{
  glBindVertexArray(0);
}

GLLayout gfx_add_vertex_layout(GLObject *vertex_array, u32 count, GLenum type)
{
  u32 type_size;

  switch (type)
  {
    case GL_BYTE: {type_size = sizeof (i8);} break;
    case GL_SHORT: {type_size = sizeof (i16);} break;
    case GL_INT: {type_size = sizeof (i32);} break;
    case GL_FLOAT: {type_size = sizeof (f32);} break;
    default: type_size = 1;
  }

  GLLayout layout = {
    .index = vertex_array->attrib_index,
    .count = count,
    .data_type = type,
    .normalized = false,
    .stride = count * vertex_array->attrib_count * type_size,
    .first = (void *) (u64) (vertex_array->attrib_index * count * type_size)
  };

  vertex_array->attrib_index++;

  return layout;
}

void gfx_set_vertex_layout(GLObject *vertex_array, GLLayout *layout)
{
  GFX_GL_ASSERT(glVertexAttribPointer(
                                      layout->index,
                                      layout->count,
                                      layout->data_type,
                                      layout->normalized,
                                      layout->stride,
                                      layout->first));

  GFX_GL_ASSERT(glEnableVertexAttribArray(layout->index));
}

// General ---------------------------------------------------------------------

void gfx_clear(Vec4F color)
{
  glClearColor(color.r, color.g, color.b, color.a);
  glClear(GL_COLOR_BUFFER_BIT);
}
