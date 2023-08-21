// https://learnopengl.com/Getting-started

#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>

#include "glad/glad.h"

#include "common.hpp"
#include "shaders.hpp"
#include "gfx.hpp"

struct State
{
  bool running;
  bool first_frame;
};

// #define LOG_PERF

#define CENTERED SDL_WINDOWPOS_CENTERED
#define WINDOW_FLAGS SDL_WINDOW_OPENGL

static void handle_input(SDL_Event *event, bool *running);

i32 main()
{
  State state;
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
  
  SDL_GL_SetSwapInterval(0);

  gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress);

  Shader shader = gfx_create_shader(vert_shader_source, frag_shader_source);

  f32 vertices[] = {
    -0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f, // top left
     0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f, // top right
     0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f,  1.0f, 1.0f, 0.0f, // bottom left
  };

  u8 indices[] = {
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
  };

  // Tells OpenGL how it should interpret the vertex data
  VertexArray va = gfx_create_array();
  gfx_bind_array(&va);

  Buffer vb = gfx_create_buffer(vertices, sizeof (vertices), BufferType::VERTEX);
  gfx_bind_buffer(&vb);

  Buffer ib = gfx_create_buffer(indices, sizeof (indices), BufferType::INDEX);
  gfx_bind_buffer(&ib);

  VertexAttribute pos_attrib = {
    .index = 0,
    .size = 3,
    .type = GL_FLOAT,
    .normalized = false,
    .stride = 6 * sizeof (f32),
    .first = (void *) 0
  };

  VertexAttribute color_attrib = {
    .index = 1,
    .size = 3,
    .type = GL_FLOAT,
    .normalized = false,
    .stride = 6 * sizeof (f32),
    .first = (void *) (3 * sizeof (f32))
  };

  gfx_set_vertex_attrib(&pos_attrib);
  gfx_set_vertex_attrib(&color_attrib);

  // Unbind everything
  gfx_unbind_array();
  gfx_unbind_shader();
  gfx_unbind_buffer(&vb);
  gfx_unbind_buffer(&ib);

  state.running = true;
  state.first_frame = true;

  while (state.running)
  {
    u64 frame_start = SDL_GetTicks64();

    // Handle events
    if (!state.first_frame)
    {
      SDL_Event event;
      while (SDL_PollEvent(&event))
      {
        handle_input(&event, &state.running);
      }
    }
    
    gfx_clear((Vec4F) {0.1f, 0.1f, 0.1f, 1.0f});

    // Draw rectangle
    gfx_bind_shader(&shader);
    gfx_bind_array(&va);
    GFX_GL_ASSERT(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (void *) 0));

    // Swap the front and back buffers to make the rendered image visible
    SDL_GL_SwapWindow(window);

    state.first_frame = false;

    u64 frame_end = SDL_GetTicks64();
    u64 frame_time = frame_end - frame_start;

    #ifdef LOG_PERF
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
