// https://learnopengl.com/Getting-started/Hello-Triangle

#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>

#include "glad/glad.h"

#include "common.hpp"
#include "shaders.hpp"
#include "gfx.hpp"

// #define PERF_LOG

#define CENTERED SDL_WINDOWPOS_CENTERED
#define WINDOW_FLAGS SDL_WINDOW_OPENGL

static void handle_input(SDL_Event *event, bool *running);

i32 main()
{
  SDL_Window *window;
  SDL_GLContext context;

  SDL_InitSubSystem(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);

  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 0);

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

  window = SDL_CreateWindow("OPENGL", CENTERED, CENTERED, 800, 450, WINDOW_FLAGS);
  context = SDL_GL_CreateContext(window);

  SDL_GL_MakeCurrent(window, context);
  SDL_GL_SetSwapInterval(1);

  gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress);

  Shader shader = gfx_create_shader(vert_shader_source, frag_shader_source);

  f32 vertices[] = {
    -0.5f, 0.5f, 0.0f,  1.0f, 0.0f, 0.0f, // top left
     0.5f, 0.5f, 0.0f,  0.0f, 0.0f, 1.0f, // top right
     0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, // bottom left
  };

  // Vertex buffer object stores vertex data that is uploaded to GPU
  u32 vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof (vertices), vertices, GL_STATIC_DRAW);

  // Vertex array object stores the state needed to supply vertex data
  // Tells OpenGL how it should interpret the vertex data
  u32 vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // Position attribute
  glVertexAttribPointer(0, 3, GL_INT, false, 6 * sizeof (f32), (void *) 0);
  glEnableVertexAttribArray(0);

  // Color attrubute
  glVertexAttribPointer(1, 3, GL_FLOAT, false, 6 * sizeof (f32), (void *) (3 * sizeof (f32)));
  glEnableVertexAttribArray(1);
  glBindVertexArray(0);

  u8 indices[] = {
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
  };

  u32 ebo;
  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof (indices), indices, GL_STATIC_DRAW);

  bool running = true;
  bool first_frame = true;

  while (running)
  {
    u64 frame_start = SDL_GetTicks64();

    // Handle events
    if (!first_frame)
    {
      SDL_Event event;
      while (SDL_PollEvent(&event))
      {
        handle_input(&event, &running);
      }
    }
    
    gfx_clear((Vec4F) {0.1f, 0.1f, 0.1f, 1.0f});

    gfx_use_shader(&shader);

    // Draw rectangle
    glBindVertexArray(vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    GFX_GL_ASSERT(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (void *) 0));

    // Swap the front and back buffers to make the rendered image visible
    SDL_GL_SwapWindow(window);

    first_frame = false;

    u64 frame_end = SDL_GetTicks64();
    u64 frame_time = frame_end - frame_start;

    #ifdef PERF_LOG
    printf("%lu ms\n", frame_time);
    #endif
  }

  return 0;
}

static
void handle_input(SDL_Event *event, bool *running)
{
  switch (event->type)
  {
    case SDL_QUIT: { *running = false; } break;
    case SDL_KEYDOWN: 
    {
      switch (event->key.keysym.scancode)
      {
        case SDL_SCANCODE_ESCAPE: { *running = false; } break;
        default: break;
      }
      break;
    }
    default: break;
  }
}
