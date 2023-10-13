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
  bool running : 1;
  bool first_frame : 1;
};

typedef struct Input Input;
struct Input
{
  u8 w;
  u8 a;
  u8 s;
  u8 d;
  u8 space;
  u8 escape;
};

typedef struct Transform2D Transform2D;
struct Transform2D
{
  Vec2F pos;
  Vec2F dir;
  Vec2F scale;
  f32 rot;
  Vec4F color;
};

static void set_gl_attributes(void);
static void handle_input(State *state, SDL_Event *event);

Input *input;

i32 main(void)
{
  State state;
  SDL_Window *window;
  SDL_GLContext context;

  input = malloc(sizeof (Input));

  SDL_InitSubSystem(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

  set_gl_attributes();

  window = SDL_CreateWindow("OPENGL", CENTERED, CENTERED, WIDTH, HEIGHT, FLAGS);
  context = SDL_GL_CreateContext(window);
  SDL_GL_MakeCurrent(window, context);
  SDL_GL_SetSwapInterval(VSYNC_ON);

  gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress);

  R_Shader shader = r_create_shader(shaders_vert_src, shaders_frag_src);

  R_Vertex vertices[4] = 
  {
    {{-10.0f,  10.0f, 1.0f},  {0.0f, 0.0f, 0.0f}}, // top left
    {{ 10.0f,  10.0f, 1.0f},  {0.0f, 0.0f, 0.0f}}, // top right
    {{ 10.0f, -10.0f, 1.0f},  {0.0f, 0.0f, 0.0f}}, // bottom right
    {{-10.0f, -10.0f, 1.0f},  {0.0f, 0.0f, 0.0f}}  // bottom left
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

  Transform2D player = {0};
  player.scale = v2f(1.5f, 1.5f);
  player.color = v4f(3.0f, 2.0f, 7.0f, 1.0f);

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

    if (input->escape)
    {
      state.running = FALSE;
      break;
    }

    {
      // UPDATE
      u64 t = SDL_GetTicks64();

      // Object
      Mat3x3F sprite = m3x3f(1.0f);
      sprite = mul_3x3f(scale_3x3f(sin(t * 0.005f) * 5.0f, 5.0f), sprite);
      sprite = mul_3x3f(rotate_3x3f(t * 0.1f), sprite);

      // Player
      if (input->a) player.dir.x = -1.0f;
      if (input->d) player.dir.x = 1.0f;
      if (input->w) player.dir.y = -1.0f;
      if (input->s) player.dir.y = 1.0f;
      if ((!input->a && !input->d) || (input->a && input->d)) player.dir.x = 0.0f;
      if ((!input->w && !input->s) || (input->w && input->s)) player.dir.y = 0.0f;

      player.pos = add_2f(player.pos, scale_2f(player.dir, 3.0f));

      Mat3x3F p_sprite = m3x3f(1.0f);
      p_sprite = mul_3x3f(scale_3x3f(player.scale.x, player.scale.y), p_sprite);
      p_sprite = mul_3x3f(rotate_3x3f(player.rot), p_sprite);
      p_sprite = mul_3x3f(translate_3x3f(player.pos.x, -player.pos.y), p_sprite);

      Mat3x3F camera = m3x3f(1.0f);
      camera = mul_3x3f(translate_3x3f(WIDTH / 2.0f, HEIGHT / 2.0f), camera);

      Mat3x3F projection = m3x3f(1.0f);
      projection = mul_3x3f(orthographic_3x3f(0.0f, WIDTH, 0.0f, HEIGHT), projection);

      // DRAW
      r_clear(v4f(0.1f, 0.1f, 0.1f, 1.0f));

      // Object
      Mat3x3F scp = mul_3x3f(mul_3x3f(projection, camera), sprite);
      r_set_uniform_3x3f(&shader, "u_xform", scp);
      r_set_uniform_4f(&shader, "u_color", v4f(1.0f, 0.0f, 0.0f, 1.0f));
      r_draw(&vert_arr, &shader);
      
      // Player
      Mat3x3F player_scp = mul_3x3f(mul_3x3f(projection, camera), p_sprite);
      r_set_uniform_3x3f(&shader, "u_xform", player_scp);
      r_set_uniform_4f(&shader, "u_color", player.color);
      r_draw(&vert_arr, &shader);

      SDL_GL_SwapWindow(window);
    }

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
        default: break;
        case SDL_SCANCODE_ESCAPE: { input->escape = TRUE; } break;
        case SDL_SCANCODE_SPACE: { input->space = TRUE; } break;
        case SDL_SCANCODE_A: { input->a = TRUE; } break;
        case SDL_SCANCODE_D: { input->d = TRUE; } break;
        case SDL_SCANCODE_S: { input->s = TRUE; } break;
        case SDL_SCANCODE_W: { input->w = TRUE; } break;
        break;
      }
      break;
    }
    case SDL_KEYUP: 
    {
      switch (event->key.keysym.scancode)
      {
        default: break;
        case SDL_SCANCODE_ESCAPE: { input->escape = FALSE; } break;
        case SDL_SCANCODE_SPACE: { input->space = FALSE; } break;
        case SDL_SCANCODE_A: { input->a = FALSE; } break;
        case SDL_SCANCODE_D: { input->d = FALSE; } break;
        case SDL_SCANCODE_S: { input->s = FALSE; } break;
        case SDL_SCANCODE_W: { input->w = FALSE; } break;
        break;
      }
      break;
    }
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
