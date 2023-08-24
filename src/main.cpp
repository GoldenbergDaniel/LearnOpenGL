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

#define DEBUG
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

  window = SDL_CreateWindow("OPENGL", CENTERED, CENTERED, 800, 450, SDL_WINDOW_OPENGL);
  context = SDL_GL_CreateContext(window);
  SDL_GL_MakeCurrent(window, context);
  
  SDL_GL_SetSwapInterval(0);

  gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress);

  String vert_shader_str = str_lit(vert_shader_source);
  String frag_shader_str = str_lit(frag_shader_source);

  gfx::GLObject shader = gfx::create_shader(&vert_shader_str, &frag_shader_str);

  f32 vertices[] = {
    -0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f, // top left
     0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f, // top right
     0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f,  1.0f, 1.0f, 0.0f, // bottom left
  };

  u16 indices[] = {
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
  };

  gfx::GLObject v_arr = gfx::create_vertex_array(2);
  gfx::GLObject v_buf = gfx::create_vertex_buffer(vertices, sizeof (vertices));
  gfx::GLObject i_buf = gfx::create_index_buffer(indices, sizeof (indices));

  gfx::bind(&v_arr);
  gfx::bind(&v_buf);
  gfx::bind(&i_buf);

  gfx::GLLayout position_layout = gfx::add_vertex_layout(&v_arr, 3, GL_FLOAT);
  gfx::GLLayout color_layout = gfx::add_vertex_layout(&v_arr, 3, GL_FLOAT);

  gfx::set_vertex_layout(&v_arr, &position_layout);
  gfx::set_vertex_layout(&v_arr, &color_layout);

  // Unbind everything
  gfx::unbind_shader();
  gfx::unbind_vertex_array();
  gfx::unbind_vertex_buffer();
  gfx::unbind_index_buffer();

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
    
    gfx::clear(vec4f(0.1f, 0.1f, 0.1f, 1.0f));

    // Draw rectangle
    gfx::bind(&shader);
    gfx::bind(&v_arr);
    GFX_GL_ASSERT(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void *) 0));

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
