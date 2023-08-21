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

  GLObject shader = gfx_create_shader(vert_shader_source, frag_shader_source);

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

  GLObject va = gfx_create_vertex_array();
  GLObject vb = gfx_create_vertex_buffer(vertices, sizeof (vertices));
  GLObject ib = gfx_create_index_buffer(indices, sizeof (indices));

  gfx_bind_vertex_array(&va);
  gfx_bind_vertex_buffer(&vb);
  gfx_bind_index_buffer(&ib);

  GLAttribute pos_attrib = gfx_create_vertex_attrib(0, 3, GL_FLOAT);
  GLAttribute color_attrib = gfx_create_vertex_attrib(1, 3, GL_FLOAT);

  gfx_set_vertex_attrib(&va, &pos_attrib);
  gfx_set_vertex_attrib(&va, &color_attrib);

  // Unbind everything
  gfx_unbind_shader();
  gfx_unbind_vertex_buffer();
  gfx_unbind_index_buffer();

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
    gfx_bind_vertex_array(&va);
    GFX_GL_ASSERT(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (void *) 0));

    // Swap front and back buffers of window
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
