#include <stdio.h>
#include <alloca.h>
#include <assert.h>
#include <SDL2/SDL.h>

#include "glad/glad.h"

#include "common.hpp"
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

GLObject gfx_create_shader(const i8 *vert_src, const i8 *frag_src)
{
  u32 vert = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vert, 1, &vert_src, nullptr);
  glCompileShader(vert);
  gfx_verify_shader(vert, GL_COMPILE_STATUS);

  u32 frag = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(frag, 1, &frag_src, nullptr);
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

// Vertex Buffer

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

// Vertex Array

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

GLObject gfx_create_vertex_array()
{
  u32 id;
  glGenVertexArrays(1, &id);

  return (GLObject) {id};
}

GLAttribute gfx_create_vertex_attrib(u32 index, u32 count, GLenum data_type)
{
  u32 data_type_size = 1;

  switch (data_type)
  {
    case GL_BYTE: {data_type_size = sizeof (i8);} break;
    case GL_SHORT: {data_type_size = sizeof (i16);} break;
    case GL_INT: {data_type_size = sizeof (i32);} break;
    case GL_FLOAT: {data_type_size = sizeof (f32);} break;
    case GL_DOUBLE: {data_type_size = sizeof (f64);} break;
    default: break;
  }

  GLAttribute attrib = {
    .index = index,
    .count = count,
    .data_type = data_type,
    .normalized = false,
    .stride = count * ATTRIBUTE_COUNT * data_type_size,
    .first = (void *) (u64) (index * count * data_type_size)
  };

  return attrib;
}

// NOTE: Binding vertex array could be expensive. Maybe bind seperately?
void gfx_set_vertex_attrib(GLObject *vertex_array, GLAttribute *attrib)
{
  GFX_GL_ASSERT(glBindVertexArray(vertex_array->id));
  GFX_GL_ASSERT(glVertexAttribPointer(
                        attrib->index,
                        attrib->count,
                        attrib->data_type,
                        attrib->normalized, 
                        attrib->stride,
                        attrib->first));

  GFX_GL_ASSERT(glEnableVertexAttribArray(attrib->index));
  GFX_GL_ASSERT(glBindVertexArray(0));
}

void gfx_bind_vertex_array(GLObject *array)
{
  GFX_GL_ASSERT(glBindVertexArray(array->id));
}

void gfx_unbind_vertex_array()
{
  glBindVertexArray(0);
}

void gfx_clear(Vec4F color)
{
  glClearColor(color.r, color.g, color.b, color.a);
  glClear(GL_COLOR_BUFFER_BIT);
}
