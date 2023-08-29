// https://learnopengl.com/Getting-started

#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>

#include "glad/glad.h"

#include "common.h"
#include "shaders.h"
#include "render.h"

struct State
{
  bool running;
  bool first_frame;
};

#define DEBUG
// #define LOG_PERF

#define CENTERED SDL_WINDOWPOS_CENTERED
#define WINDOW_FLAGS SDL_WINDOW_OPENGL

static void handle_input(State *state, SDL_Event *event);

i32 main()
{
  State state;
  state.running = true;

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

  R_Shader shader = r_create_shader(v_shader_src, f_shader_src);

  R_Vertex vertices[4] =
  {
    {{-0.5f,  0.5f, 0.0f},  {1.0f, 0.0f, 0.0f}}, // top left
    {{0.5f,   0.5f, 0.0f},  {0.0f, 0.0f, 1.0f}}, // top right
    {{0.5f,  -0.5f, 0.0f},  {0.0f, 1.0f, 0.0f}}, // bottom right
    {{-0.5f, -0.5f, 0.0f},  {1.0f, 1.0f, 0.0f}} // bottom left
  };

  u16 indices[6] = 
  {
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
  };

  R_Object v_arr = r_create_vertex_array(2);
  R_Object v_buf = r_create_buffer(R_BufferType_V, vertices, sizeof (vertices));
  R_Object i_buf = r_create_buffer(R_BufferType_I, indices, sizeof (indices));

  r_bind_vertex_array(&v_arr);
  r_bind_buffer(R_BufferType_V, &v_buf);
  r_bind_buffer(R_BufferType_I, &i_buf);

  R_Layout position_layout = r_add_vertex_layout(&v_arr, GL_FLOAT, 3);
  R_Layout color_layout = r_add_vertex_layout(&v_arr, GL_FLOAT, 3);

  r_set_vertex_layout(&v_arr, &position_layout);
  r_set_vertex_layout(&v_arr, &color_layout);

  // Unbind everything
  r_unbind_shader();
  r_unbind_vertex_array();
  r_unbind_buffer(R_BufferType_V);
  r_unbind_buffer(R_BufferType_I);

  state.running = true;
  state.first_frame = true;

  // Main loop
  while (state.running)
  {
    u64 frame_start = SDL_GetTicks64();

    // Handle events
    if (!state.first_frame)
    {
      SDL_Event event;
      while (SDL_PollEvent(&event))
      {
        handle_input(&state, &event);
      }
    }

    // Update position
    // r_set_uniform(&shader, (i8 *) "u_pos", vec2f(1.f, 1.f));

    d_clear(vec4f(0.1f, 0.1f, 0.1f, 1.0f));

    // Draw rectangle
    r_bind_shader(&shader);
    r_bind_vertex_array(&v_arr);
    R_ASSERT(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void *) 0));

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
void handle_input(State *state, SDL_Event *event)
{
  switch (event->type)
  {
    case SDL_QUIT: { state->running = false; } break;
    case SDL_KEYDOWN: 
    {
      switch (event->key.keysym.scancode)
      {
        case SDL_SCANCODE_ESCAPE: { state->running = false; } break;
        default: break;
      }
      break;
    }
    default: break;
  }
}
