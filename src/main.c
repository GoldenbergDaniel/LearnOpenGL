#include <SDL2/SDL.h>

#include "glad/glad.h"

#include "base_common.h"
#include "base_math.h"
#include "shaders.h"
#include "render.h"

#define DEBUG
// #define LOG_PERF

typedef struct State State;
struct State
{
  bool running;
  bool first_frame;
};

static void set_gl_attributes(void);
static void handle_input(State *state, SDL_Event *event);

i32 main(void)
{
  State state;
  SDL_Window *window;
  SDL_GLContext context;

  SDL_InitSubSystem(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

  set_gl_attributes();

  window = SDL_CreateWindow("OPENGL", CENTERED, CENTERED, WIDTH, HEIGHT, FLAGS);
  context = SDL_GL_CreateContext(window);
  SDL_GL_MakeCurrent(window, context);
  SDL_GL_SetSwapInterval(VSYNC_OFF);

  gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress);

  R_Shader shader = r_create_shader(v_shader_src, f_shader_src);

  R_Vertex vertices[4] = 
  {
    {{-50.0f,  50.0f, 1.0f},  {1.0f, 0.0f, 0.0f}}, // top left
    {{ 50.0f,  50.0f, 1.0f},  {0.0f, 0.0f, 1.0f}}, // top right
    {{ 50.0f, -50.0f, 1.0f},  {0.0f, 1.0f, 0.0f}}, // bottom right
    {{-50.0f, -50.0f, 1.0f},  {1.0f, 1.0f, 0.0f}}  // bottom left
  };

  u16 indices[6] = 
  {
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
  };

  R_Object vert_arr = r_create_vertex_array(2);
  r_create_vertex_buffer(vertices, sizeof (vertices));
  r_create_index_buffer(indices, sizeof (indices));

  R_VertexLayout vert_pos_layout = r_create_vertex_layout(&vert_arr, GL_FLOAT, 3);
  r_bind_vertex_layout(&vert_pos_layout);
  
  R_VertexLayout vert_col_layout = r_create_vertex_layout(&vert_arr, GL_FLOAT, 3);
  r_bind_vertex_layout(&vert_col_layout);

  state.running = TRUE;
  state.first_frame = TRUE;

  // Main loop
  while (state.running)
  {
    #ifdef LOG_PERF
    u64 frame_start = SDL_GetPerformanceCounter();
    #endif

    // Handle events
    if (!state.first_frame)
    {
      SDL_Event event;
      while (SDL_PollEvent(&event))
      {
        handle_input(&state, &event);
      }
    }

    // Update
    u64 t = SDL_GetTicks64();

    // Object 1
    Mat3x3F sprite = m3x3f(1.0f);
    sprite = mul_3x3f(shear_3x3f(sin(t * 0.005f), cos(t * 0.005f)), sprite);
    sprite = mul_3x3f(rotate_3x3f(t * 0.05f), sprite);

    // Object 2
    Mat3x3F sprite2 = m3x3f(1.0f);
    sprite2 = mul_3x3f(translate_3x3f(200.0f, 100.0f), sprite2);

    Mat3x3F camera = m3x3f(1.0f);
    camera = mul_3x3f(translate_3x3f(WIDTH/2.0f, HEIGHT/2.0f), camera);

    Mat3x3F projection = m3x3f(1.0f);
    projection = mul_3x3f(orthographic_3x3f(0.0f, WIDTH, 0.0f, HEIGHT), projection);

    // Draw
    r_clear(v4f(0.1f, 0.1f, 0.1f, 1.0f));

    // Object 1
    Mat3x3F scp = mul_3x3f(mul_3x3f(projection, camera), sprite);
    r_set_uniform_3x3f(&shader, "u_xform", scp);
    r_draw(&vert_arr, &shader);
    
    // Object 2
    Mat3x3F scp2 = mul_3x3f(mul_3x3f(projection, camera), sprite2);
    r_set_uniform_3x3f(&shader, "u_xform", scp2);
    r_draw(&vert_arr, &shader);

    // Swap front and back buffers of window
    SDL_GL_SwapWindow(window);

    state.first_frame = FALSE;

    #ifdef LOG_PERF
    u64 frame_end = SDL_GetPerformanceCounter();
    u64 frequency = SDL_GetPerformanceFrequency();
    f64 frame_time = (f64) (frame_end - frame_start) / frequency * 1000.0f;

    printf("%.2lf ms\n", frame_time);
    #endif
  }

  SDL_DestroyWindow(window);
  SDL_Quit();

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

static
void set_gl_attributes(void)
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
