#include <stdio.h>
#include <alloca.h>
#include <assert.h>
#include <SDL2/SDL.h>

#include "glad/glad.h"

#include "common.hpp"
#include "util.hpp"
#include "render.hpp"

static void r_verify_shader(u32 id, GLenum type);

bool _r_check_error()
{
  while (u32 err = glGetError())
  {
    printf("[OpenGL Error]: %u\n", err);
    return false;
  }

  return true;
}

void _r_clear_error()
{
  while (glGetError() != GL_NO_ERROR);
}

// Shader ----------------------------------------------------------------------

R_Shader r_create_shader(i8 *vert_src, i8 *frag_src)
{
  u32 vert = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vert, 1, &vert_src, nullptr);
  glCompileShader(vert);
  r_verify_shader(vert, GL_COMPILE_STATUS);

  u32 frag = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(frag, 1, &frag_src, nullptr);
  glCompileShader(frag);
  r_verify_shader(frag, GL_COMPILE_STATUS);

  u32 id = glCreateProgram();
  glAttachShader(id, frag);
  glAttachShader(id, vert);
  glLinkProgram(id);
  r_verify_shader(id, GL_LINK_STATUS);

  glDeleteShader(vert);
  glDeleteShader(frag);

  return (R_Shader) {id};
}

void r_bind_shader(R_Shader *shader)
{
  R_ASSERT(glUseProgram(shader->id));
}

void r_unbind_shader()
{
  glUseProgram(0);
}

i32 r_set_uniform(R_Shader *shader, i8 *name, u32 val)
{
  i32 loc = glGetUniformLocation(shader->id, name);
  glUniform1ui(loc, val);
  
  return loc;
}

i32 r_set_uniform(R_Shader *shader, i8 *name, i32 val)
{
  i32 loc = glGetUniformLocation(shader->id, name);
  glUniform1i(loc, val);
  
  return loc;
}

i32 r_set_uniform(R_Shader *shader, i8 *name, f32 val)
{
  i32 loc = glGetUniformLocation(shader->id, name);
  glUniform1f(loc, val);
  
  return loc;
}

i32 r_set_uniform(R_Shader *shader, i8 *name, Vec2F vec)
{
  i32 loc = glGetUniformLocation(shader->id, name);
  glUniform2f(loc, vec.x, vec.y);

  return loc;
}

i32 r_set_uniform(R_Shader *shader, i8 *name, Vec3F vec)
{
  i32 loc = glGetUniformLocation(shader->id, name);
  glUniform3f(loc, vec.x, vec.y, vec.z);
  
  return loc;
}

i32 r_set_uniform(R_Shader *shader, i8 *name, Vec4F vec)
{
  i32 loc = glGetUniformLocation(shader->id, name);
  glUniform4f(loc, vec.x, vec.y, vec.z, vec.w);
  
  return loc;
}

i32 r_set_uniform(R_Shader *shader, String *name, Mat4x4F mat)
{
  // NOTE: Column-major order
  f32 matrix[16];
  matrix[0] = mat.r1.x;
  matrix[1] = mat.r2.x;
  matrix[2] = mat.r3.x;
  matrix[3] = mat.r4.x;
  matrix[4] = mat.r1.y;
  matrix[5] = mat.r2.y;
  matrix[6] = mat.r3.y;
  matrix[7] = mat.r4.y;
  matrix[8] = mat.r1.z;
  matrix[9] = mat.r2.z;
  matrix[10] = mat.r3.z;
  matrix[11] = mat.r4.z;
  matrix[12] = mat.r1.w;
  matrix[13] = mat.r2.w;
  matrix[14] = mat.r3.w;
  matrix[15] = mat.r4.w;

  i32 loc = glGetUniformLocation(shader->id, name->data);
  glUniformMatrix4fv(loc, 16, false, matrix);
  
  return loc;
}

static
void r_verify_shader(u32 id, GLenum type)
{
  i32 success;

  if (type == GL_LINK_STATUS)
  {
    glValidateProgram(id);
  }

  glGetShaderiv(id, type, &success);

  if (!success)
  {
    i32 length;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
    i8 *log = (i8 *) alloca(length);
    glGetShaderInfoLog(id, length, &length, log);

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

R_Object r_create_vertex_buffer(void *data, u32 size)
{
  u32 id;
  glGenBuffers(1, &id);
  R_ASSERT(glBindBuffer(GL_ARRAY_BUFFER, id));
  R_ASSERT(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));

  return (R_Object) {id};
}

void r_bind_vertex_buffer(R_Object *vertex_buffer)
{
  R_ASSERT(glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer->id));
}

void r_unbind_vertex_buffer()
{
  R_ASSERT(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

// Index Buffer ----------------------------------------------------------------

R_Object r_create_index_buffer(void *data, u32 size)
{
  u32 id;
  glGenBuffers(1, &id);
  R_ASSERT(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id));
  R_ASSERT(glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));

  return (R_Object) {id};
}

void r_bind_index_buffer(R_Object *vertex_buffer)
{
  R_ASSERT(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertex_buffer->id));
}

void r_unbind_index_buffer()
{
  R_ASSERT(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

// Vertex Array ----------------------------------------------------------------

R_Object r_create_vertex_array(u32 attrib_count)
{
  u32 id;
  glGenVertexArrays(1, &id);

  return (R_Object) {id, attrib_count, 0};
}

void r_bind_vertex_array(R_Object *vertex_array)
{
  R_ASSERT(glBindVertexArray(vertex_array->id));
}

void r_unbind_vertex_array()
{
  glBindVertexArray(0);
}

R_Layout r_add_vertex_layout(R_Object *vertex_array, GLenum type, u32 count)
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

  R_Layout layout = {
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

void r_set_vertex_layout(R_Object *vertex_array, R_Layout *layout)
{
  R_ASSERT(glVertexAttribPointer(
                                   layout->index,
                                   layout->count,
                                   layout->data_type,
                                   layout->normalized,
                                   layout->stride,
                                   layout->first));

  R_ASSERT(glEnableVertexAttribArray(layout->index));
}

// General ---------------------------------------------------------------------

void r_clear(Vec4F color)
{
  glClearColor(color.r, color.g, color.b, color.a);
  glClear(GL_COLOR_BUFFER_BIT);
}