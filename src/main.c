// https://learnopengl.com/Getting-started

#include <stdio.h>
#include <SDL2/SDL.h>

#include "glad/glad.h"

#include "base_common.h"
#include "base_math.h"
#include "shaders.h"
#include "render.h"

typedef struct State State;
struct State
{
  b8 running;
  b8 first_frame;
};

#define DEBUG
// #define LOG_PERF

#define CENTERED SDL_WINDOWPOS_CENTERED
#define WINDOW_FLAGS SDL_WINDOW_OPENGL

static void set_gl_attributes();
static void handle_input(State *state, SDL_Event *event);

i32 main(void)
{
  State state;
  SDL_Window *window;
  SDL_GLContext context;

  SDL_InitSubSystem(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

  set_gl_attributes();

  window = SDL_CreateWindow("OPENGL", CENTERED, CENTERED, 800, 450, WINDOW_FLAGS);
  context = SDL_GL_CreateContext(window);
  SDL_GL_MakeCurrent(window, context);
  
  SDL_GL_SetSwapInterval(VSYNC_ON);

  gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress);

  R_Shader shader = r_create_shader(v_shader_src, f_shader_src);

  R_Vertex vertices[4] = 
  {
    {{100.0f, 200.0f, 0.0f},  {1.0f, 0.0f, 0.0f}}, // top left
    {{200.0f, 200.0f, 0.0f},  {0.0f, 0.0f, 1.0f}}, // top right
    {{200.0f, 100.0f, 0.0f},  {0.0f, 1.0f, 0.0f}}, // bottom right
    {{100.0f, 100.0f, 0.0f},  {1.0f, 1.0f, 0.0f}}  // bottom left
  };

  u16 indices[6] = 
  {
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
  };

  R_Object vert_arr = r_create_vertex_array(2);
  r_bind_vertex_array(&vert_arr);

R_Object vert_buf = r_create_vertex_buffer(vertices, sizeof (vertices));
  R_Object index_buf = r_create_index_buffer(indices, sizeof (indices));

  R_Layout vert_pos_layout = r_add_vertex_layout(&vert_arr, GL_FLOAT, 3);
  r_set_vertex_layout(&vert_arr, &vert_pos_layout);
  
  R_Layout vert_col_layout = r_add_vertex_layout(&vert_arr, GL_FLOAT, 3);
  r_set_vertex_layout(&vert_arr, &vert_col_layout);

  // Orthographic Projection
  Mat4x4F proj = orthographic_4x4f(0.0f, 800.0f, 0.0f, 450.0f);
  r_bind_shader(&shader);
  r_set_uniform_4x4f(&shader, "u_proj", proj);

  state.running = TRUE;
  state.first_frame = TRUE;

  // Main loop
  while (state.running)
  {
    u64 frame_start = SDL_GetPerformanceCounter();

    // Handle events
    if (!state.first_frame)
    {
      SDL_Event event;
      while (SDL_PollEvent(&event))
      {
        handle_input(&state, &event);
      }
    }

    // Draw
    r_clear(v4f(0.1f, 0.1f, 0.1f, 1.0f));
    r_draw(&vert_arr, &shader);

    // Swap front and back buffers of window
    SDL_GL_SwapWindow(window);

    state.first_frame = FALSE;

    u64 frame_end = SDL_GetPerformanceCounter();
    u64 frequency = SDL_GetPerformanceFrequency();
    f64 frame_time = (f64) (frame_end - frame_start) / frequency * 1000.0f;

    #ifdef LOG_PERF
    printf("%.2lf ms\n", frame_time);
    #endif
  }

  return 0;
}

static
void handle_input(State *state, SDL_Event *event)
{
  switch (event->type)
  {
    case SDL_QUIT: { state->running = FALSE; } break;
    case SDL_KEYDOWN: 
    {
      switch (event->key.keysym.scancode)
      {
        case SDL_SCANCODE_ESCAPE: { state->running = FALSE; } break;
        default: break;
      }
      break;
    }
    default: break;
  }
}

static inline
void set_gl_attributes()
{
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
}
