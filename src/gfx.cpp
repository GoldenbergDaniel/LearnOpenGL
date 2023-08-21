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

Shader gfx_create_shader(const i8 *vert_src, const i8 *frag_src)
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

  return (Shader) {id};
}

void gfx_bind_shader(Shader *shader)
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
      printf("[Shader Error]: Failed to compile shader!\n");
    }
    else
    {
      printf("[Shader Error]: Failed to link shaders!\n");
    }

    printf("%s", log);
  }
}

Buffer gfx_create_buffer(void *data, u32 size, BufferType type)
{
  u32 id;

  glGenBuffers(1, &id);

  if (type == BufferType::VERTEX)
  {
    GFX_GL_ASSERT(glBindBuffer(GL_ARRAY_BUFFER, id));
    GFX_GL_ASSERT(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
  }
  else if (type == BufferType::INDEX)
  {
    GFX_GL_ASSERT(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id));
    GFX_GL_ASSERT(glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
  }

  return (Buffer) {id, type};
}

void gfx_bind_buffer(Buffer *buffer)
{
  if (buffer->type == BufferType::VERTEX)
  {
    GFX_GL_ASSERT(glBindBuffer(GL_ARRAY_BUFFER, buffer->id));
  }
  else if (buffer->type == BufferType::INDEX)
  {
    GFX_GL_ASSERT(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->id));
  }
}

void gfx_unbind_buffer(Buffer *buffer)
{
  if (buffer->type == BufferType::VERTEX)
  {
    GFX_GL_ASSERT(glBindBuffer(GL_ARRAY_BUFFER, 0));
  }
  else if (buffer->type == BufferType::INDEX)
  {
    GFX_GL_ASSERT(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
  }
}

VertexArray gfx_create_array()
{
  u32 id;
  glGenVertexArrays(1, &id);

  return (VertexArray) {id};
}

void gfx_set_vertex_attrib(VertexAttribute *attrib)
{
  GFX_GL_ASSERT(glVertexAttribPointer(
                        attrib->index,
                        attrib->size, 
                        attrib->type, 
                        attrib->normalized, 
                        attrib->stride,
                        attrib->first));

  GFX_GL_ASSERT(glEnableVertexAttribArray(attrib->index));
}

void gfx_bind_array(VertexArray *array)
{
  GFX_GL_ASSERT(glBindVertexArray(array->id));
}

void gfx_unbind_array()
{
  glBindVertexArray(0);
}

void gfx_clear(Vec4F color)
{
  glClearColor(color.r, color.g, color.b, color.a);
  glClear(GL_COLOR_BUFFER_BIT);
}
