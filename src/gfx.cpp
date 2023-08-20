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

void gfx_use_shader(Shader *shader)
{
  glUseProgram(shader->id);
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

void gfx_clear(Vec4F color)
{
  glClearColor(color.r, color.g, color.b, color.a);
  glClear(GL_COLOR_BUFFER_BIT);
}
