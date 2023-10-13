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
    sprite = mul_3x3f(scale_3x3f(sin(t * 0.005f), 1.0f), sprite);
    // sprite = mul_3x3f(shear_3x3f(sin(t * 0.005f), cos(t * 0.005f)), sprite);
    // sprite = mul_3x3f(rotate_3x3f(t * 0.1f), sprite);
    // sprite = mul_3x3f(reflect_3x3f(v2f(1, 0)), sprite);

    // Object 2
    Mat3x3F sprite2 = m3x3f(1.0f);
    sprite2 = mul_3x3f(translate_3x3f(200.0f, 100.0f), sprite2);

    Mat3x3F camera = m3x3f(1.0f);
    camera = mul_3x3f(translate_3x3f(WIDTH / 2.0f, HEIGHT / 2.0f), camera);

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
    case SDL_QUIT: state->running = FALSE; 
    break;
    case SDL_KEYDOWN:
    {
      switch (event->key.keysym.scancode)
      {
        case SDL_SCANCODE_ESCAPE: state->running = FALSE;
        break;
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
#include <math.h>

#include "base_common.h"
#include "base_math.h"

// @Vec2F ===================================================================================

inline
Vec2F v2f(f32 x, f32 y)
{
  return (Vec2F) {x, y};
}

inline
Vec2F add_2f(Vec2F a, Vec2F b)
{
  return (Vec2F) {a.x + b.x, a.y + b.y};
}

inline
Vec2F sub_2f(Vec2F a, Vec2F b)
{
  return (Vec2F) {a.x - b.x, a.y - b.y};
}

inline
Vec2F mul_2f(Vec2F a, Vec2F b)
{
  return (Vec2F) {a.x * b.x, a.y * b.y};
}

inline
Vec2F div_2f(Vec2F a, Vec2F b)
{
  return (Vec2F) {a.x / b.x, a.y / b.y};
}

inline
f32 dot_2f(Vec2F a, Vec2F b)
{
  return (a.x * b.x) + (a.y * b.y);
}

inline
f32 cross_2f(Vec2F a, Vec2F b)
{
  f32 result;
  result = (a.x * b.y) -(a.y * b.x);

  return result;
}

inline
Vec2F scale_2f(Vec2F v, f32 scale)
{
  return (Vec2F) {v.x * scale, v.y * scale};
}

inline
f32 magnitude_2f(Vec2F v)
{
  return sqrtf(powf(v.x, 2.0f) + powf(v.y, 2.0f));
}

inline
f32 magnitude_squared_2f(Vec2F v)
{
  return powf(v.x, 2.0f) + powf(v.y, 2.0f);
}

inline
f32 distance_2f(Vec2F a, Vec2F b)
{
  Vec2F v = sub_2f(b, a);
  return sqrtf(powf(v.x, 2.0f) + powf(v.y, 2.0f));
}

inline
f32 distance_squared_2f(Vec2F a, Vec2F b)
{
  Vec2F v = sub_2f(b, a);
  return powf(v.x, 2.0f) + powf(v.y, 2.0f);
}

inline
Vec2F normalize_2f(Vec2F v)
{
  return scale_2f(v, 1.0f / magnitude_2f(v));
}

inline
Vec2F lerp_2f(Vec2F curr, Vec2F target, f32 rate)
{
  return scale_2f(sub_2f(target, curr), rate);
}

// @Vec3F ===================================================================================

inline
Vec3F v3f(f32 x, f32 y, f32 z)
{
  return (Vec3F) {x, y, z};
}

inline
Vec3F add_3f(Vec3F a, Vec3F b)
{
  return (Vec3F) {a.x + b.x, a.y + b.y, a.z + b.z};
}

inline
Vec3F sub_3f(Vec3F a, Vec3F b)
{
  return (Vec3F) {a.x - b.x, a.y - b.y, a.z - b.z};
}

inline
Vec3F mul_3f(Vec3F a, Vec3F b)
{
  return (Vec3F) {a.x * b.x, a.y * b.y, a.z * b.z};
}

inline
Vec3F div_3f(Vec3F a, Vec3F b)
{
  return (Vec3F) {a.x / b.x, a.y / b.y, a.z / b.z};
}

inline
f32 dot_3f(Vec3F a, Vec3F b)
{
  return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

inline
Vec3F cross_3f(Vec3F a, Vec3F b)
{
  Vec3F result;
  result.x = (a.y * b.z - a.z * b.y);
  result.y = -(a.x * b.z - a.z * b.x);
  result.z = (a.x * b.y - a.y * b.x);

  return result;
}

inline
Vec3F scale_3f(Vec3F v, f32 scale)
{
  return (Vec3F) {v.x * scale, v.y * scale, v.z * scale};
}

Vec3F transform_3f(Vec3F v, Mat3x3F m)
{
  Vec3F result = {0};
  for (u8 c = 0; c < 3; c++)
  {
    result.x += m.elements[0][c] * v.elements[c];
    result.y += m.elements[1][c] * v.elements[c];
    result.z += m.elements[2][c] * v.elements[c];
  }

  return result;
}

inline
f32 magnitude_3f(Vec3F v)
{
  return sqrtf(powf(v.x, 2.0f) + powf(v.y, 2.0f) + powf(v.z, 2.0f));
}

inline
f32 magnitude_squared_3f(Vec3F v)
{
  return powf(v.x, 2.0f) + powf(v.y, 2.0f) + powf(v.z, 2.0f);
}

inline
f32 distance_3f(Vec3F a, Vec3F b)
{
  Vec3F v = sub_3f(b, a);
  return sqrtf(powf(v.x, 2.0f) + powf(v.y, 2.0f) + powf(v.z, 2.0f));
}

inline
f32 distance_squared_3f(Vec3F a, Vec3F b)
{
  Vec3F v = sub_3f(b, a);
  return powf(v.x, 2.0f) + powf(v.y, 2.0f) + powf(v.z, 2.0f);
}

inline
Vec3F normalize_3f(Vec3F v)
{
  return scale_3f(v, 1.0f / magnitude_3f(v));
}

inline
Vec3F lerp_3f(Vec3F curr, Vec3F target, f32 rate)
{
  return scale_3f(sub_3f(target, curr), rate);
}

// @Vec4F ===================================================================================

inline
Vec4F v4f(f32 x, f32 y, f32 z, f32 w)
{
  return (Vec4F) {x, y, z, w};
}

inline
Vec4F add_4f(Vec4F a, Vec4F b)
{
  return (Vec4F) {a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};
}

inline
Vec4F sub_4f(Vec4F a, Vec4F b)
{
  return (Vec4F) {a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w};
}

inline
Vec4F mul_4f(Vec4F a, Vec4F b)
{
  return (Vec4F) {a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w};
}

inline
Vec4F div_4f(Vec4F a, Vec4F b)
{
  return (Vec4F) {a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w};
}

inline
f32 dot_4f(Vec4F a, Vec4F b)
{
  return (a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w);
}

inline
Vec4F scale_4f(Vec4F v, f32 scale)
{
  return (Vec4F) {v.x * scale, v.y * scale, v.z * scale, v.w * scale};
}

Vec4F transform_4f(Vec4F v, Mat4x4F m)
{
  Vec4F result = {0};
  \
  for (u8 c = 0; c < 4; c++)
  {
    result.x += m.elements[0][c] * v.elements[c];
    result.y += m.elements[1][c] * v.elements[c];
    result.z += m.elements[2][c] * v.elements[c];
    result.w += m.elements[3][c] * v.elements[c];
  }

  return result;
}

inline
f32 magnitude_4f(Vec4F v)
{
  return sqrtf(powf(v.x, 2.0f) + powf(v.y, 2.0f) + powf(v.z, 2.0f) + powf(v.z, 2.0f));
}

inline
f32 magnitude_squared_4f(Vec4F v)
{
  return powf(v.x, 2.0f) + powf(v.y, 2.0f) + powf(v.z, 2.0f) + powf(v.z, 2.0f);
}

inline
f32 distance_4f(Vec4F a, Vec4F b)
{
  Vec4F v = sub_4f(b, a);
  return sqrtf(powf(v.x, 2.0f) + powf(v.y, 2.0f) + powf(v.z, 2.0f) + powf(v.w, 2.0f));
}

inline
f32 distance_squared_4f(Vec4F a, Vec4F b)
{
  Vec4F v = sub_4f(b, a);
  return powf(v.x, 2.0f) + powf(v.y, 2.0f) + powf(v.z, 2.0f) + powf(v.w, 2.0f);
}

inline
Vec4F normalize_4f(Vec4F v)
{
  return scale_4f(v, 1.0f / magnitude_4f(v));
}

// @Mat3x3F =================================================================================

inline
Mat3x3F m3x3f(f32 k)
{
  return (Mat3x3F)
  {
    {
      {k, 0, 0},
      {0, k, 0},
      {0, 0, k}
    }
  };
}

inline
Mat3x3F rows_3x3f(Vec3F v1, Vec3F v2, Vec3F v3)
{
  return (Mat3x3F)
  {
    {
      {v1.x, v1.y, v1.z},
      {v2.x, v2.y, v2.z},
      {v3.x, v3.y, v3.z}
    }
  };
}

inline
Mat3x3F cols_3x3f(Vec3F v1, Vec3F v2, Vec3F v3)
{
  return (Mat3x3F)
  {
    {
      {v1.x, v2.x, v3.x},
      {v1.y, v2.y, v3.y},
      {v1.z, v2.z, v3.z}
    }
  };
}

Mat3x3F mul_3x3f(Mat3x3F a, Mat3x3F b)
{
  Mat3x3F result = {0};

  for (u8 r = 0; r < 3; r++)
  {
    for (u8 c = 0; c < 3; c++)
    {
      result.elements[r][c] += a.elements[r][0] * b.elements[0][c];
      result.elements[r][c] += a.elements[r][1] * b.elements[1][c];
      result.elements[r][c] += a.elements[r][2] * b.elements[2][c];
    }
  }

  return result;
}

Mat3x3F transpose_3x3f(Mat3x3F m)
{
  Mat3x3F result = m;
  result.elements[0][1] = m.elements[1][0];
  result.elements[0][2] = m.elements[2][0];
  result.elements[1][0] = m.elements[0][1];
  result.elements[1][2] = m.elements[2][1];
  result.elements[2][0] = m.elements[0][2];
  result.elements[2][1] = m.elements[1][2];

  return result;
}

Mat3x3F translate_3x3f(f32 x_shift, f32 y_shift)
{
  Mat3x3F result = m3x3f(1.0f);
  result.elements[0][2] = x_shift;
  result.elements[1][2] = y_shift;

  return result;
}

Mat3x3F rotate_3x3f(f32 angle)
{
  Mat3x3F result = m3x3f(1.0f);
  result.elements[0][0] = cos(angle * (PI / 180.0f));
  result.elements[0][1] = -sin(angle * (PI / 180.0f));
  result.elements[1][0] = sin(angle * (PI / 180.0f));
  result.elements[1][1] = cos(angle * (PI / 180.0f));

  return result;
}

Mat3x3F scale_3x3f(f32 x_scale, f32 y_scale)
{
  Mat3x3F result = m3x3f(1.0f);
  result.elements[0][0] = x_scale;
  result.elements[1][1] = y_scale;

  return result;
}

Mat3x3F shear_3x3f(f32 x_shear, f32 y_shear)
{
  Mat3x3F result = m3x3f(1.0f);
  result.elements[0][1] = x_shear;
  result.elements[1][0] = y_shear;

  return result;
}

Mat3x3F orthographic_3x3f(f32 left, f32 right, f32 bot, f32 top)
{
  Mat3x3F result = m3x3f(0.0f);
  result.elements[0][0] = 2.0f / (right - left);
  result.elements[1][1] = 2.0f / (top - bot);
  result.elements[0][2] = -(right + left) / (right - left);
  result.elements[1][2] = -(top + bot) / (top - bot);
  result.elements[2][2] = 1.0f;

  return result;
}

// @Mat4x4F =================================================================================

inline
Mat4x4F m4x4f(f32 k)
{
  return (Mat4x4F)
  {
    {
      {k, 0, 0, 0},
      {0, k, 0, 0},
      {0, 0, k, 0},
      {0, 0, 0, k}
    }
  };
}

inline
Mat4x4F rows_4x4f(Vec4F v1, Vec4F v2, Vec4F v3, Vec4F v4)
{
  return (Mat4x4F)
  {
    {
      {v1.x, v1.y, v1.z, v1.w},
      {v2.x, v2.y, v2.z, v2.w},
      {v3.x, v3.y, v3.z, v3.w},
      {v4.x, v4.y, v4.z, v4.w}
    }
  };
}

inline
Mat4x4F cols_4x4f(Vec4F v1, Vec4F v2, Vec4F v3, Vec4F v4)
{
  return (Mat4x4F)
  {
    {
      {v1.x, v2.x, v3.x, v4.x},
      {v1.y, v2.y, v3.y, v4.y},
      {v1.z, v2.z, v3.z, v4.z},
      {v1.w, v2.w, v3.w, v4.w}
    }
  };
}

Mat4x4F mul_4x4f(Mat4x4F a, Mat4x4F b)
{
  Mat4x4F result = {0};

  for (u8 r = 0; r < 4; r++)
  {
    for (u8 c = 0; c < 4; c++)
    {
      result.elements[r][c] += a.elements[0][c] * b.elements[r][0];
      result.elements[r][c] += a.elements[1][c] * b.elements[r][1];
      result.elements[r][c] += a.elements[2][c] * b.elements[r][2];
      result.elements[r][c] += a.elements[3][c] * b.elements[r][3];
    }
  }

  return result;
}

Mat4x4F transpose_4x4f(Mat4x4F m)
{
  Mat4x4F result = m;
  result.elements[0][1] = m.elements[1][0];
  result.elements[0][2] = m.elements[2][0];
  result.elements[0][3] = m.elements[3][0];
  result.elements[1][0] = m.elements[0][1];
  result.elements[1][2] = m.elements[2][1];
  result.elements[1][3] = m.elements[3][1];
  result.elements[2][0] = m.elements[0][2];
  result.elements[2][1] = m.elements[1][2];
  result.elements[2][3] = m.elements[3][2];
  result.elements[3][0] = m.elements[0][3];
  result.elements[3][1] = m.elements[1][3];
  result.elements[3][2] = m.elements[2][3];

  return result;
}

Mat4x4F translate_4x4f(f32 x_shift, f32 y_shift, f32 z_shift)
{
  Mat4x4F result = m4x4f(1.0f);
  result.elements[0][3] = x_shift;
  result.elements[1][3] = y_shift;
  result.elements[2][3] = z_shift;

  return result;
}

Mat4x4F scale_4x4f(f32 x_scale, f32 y_scale, f32 z_scale)
{
  Mat4x4F result = m4x4f(1.0f);
  result.elements[0][0] = x_scale;
  result.elements[1][1] = y_scale;
  result.elements[2][2] = z_scale;

  return result;
}

Mat4x4F orthographic_4x4f(f32 left, f32 right, f32 bot, f32 top)
{
  const f32 near = -1.0f;
  const f32 far = 1.0f;

  Mat4x4F result = {0};
  result.elements[0][0] = 2.0f / (right - left);
  result.elements[1][1] = 2.0f / (top - bot);
  result.elements[2][2] = -2.0f / (far - near);
  result.elements[0][3] = -(right + left) / (right - left);
  result.elements[1][3] = -(top + bot) / (top - bot);
  result.elements[2][3] = -(far + near) / (far - near);
  result.elements[3][3] = 1.0f;

  return result;
}

#ifdef __cplusplus

// @Overloading =============================================================================

Vec2F operator+(Vec2F a, Vec2F b)
{
  return add_2f(a, b);
}

Vec3F operator+(Vec3F a, Vec3F b)
{
  return add_3f(a, b);
}

Vec4F operator+(Vec4F a, Vec4F b)
{
  return add_4f(a, b);
}

Vec2F operator-(Vec2F a, Vec2F b)
{
  return sub_2f(a, b);
}

Vec3F operator-(Vec3F a, Vec3F b)
{
  return sub_3f(a, b);
}

Vec4F operator-(Vec4F a, Vec4F b)
{
  return sub_4f(a, b);
}

f32 operator*(Vec2F a, Vec2F b)
{
  return dot_2f(a, b);
}

f32 operator*(Vec3F a, Vec3F b)
{
  return dot_3f(a, b);
}

f32 operator*(Vec4F a, Vec4F b)
{
  return dot_4f(a, b);
}

Mat3x3F operator*(Mat3x3F a, Mat3x3F b)
{
  return mul_3x3f(a, b);
}

Mat4x4F operator*(Mat4x4F a, Mat4x4F b)
{
  return mul_4x4f(a, b);
}

#endif
#include <SDL2/SDL.h>
#include "glad/glad.h"
#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "stb/stb_image.h"

#include "base_common.h"
#include "base_math.h"
#include "render.h"

typedef R_Shader Shader;
typedef R_Object Object;
typedef R_Texture2D Texture2D;
typedef R_VertexLayout VertexLayout;

static void r_verify_shader(u32 id, GLenum type);

bool _r_check_error(void)
{
  for (u32 err = -1; (err = glGetError());)
  {
    printf("[OpenGL Error]: %u\n", err);
    return TRUE;
  }

  return FALSE;
}

void _r_clear_error(void)
{
  while (glGetError() != GL_NO_ERROR);
}

// @Shader ==================================================================================

Shader r_create_shader(const i8 *vert_src, const i8 *frag_src)
{
  u32 vert = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vert, 1, &vert_src, NULL);
  glCompileShader(vert);

  u32 frag = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(frag, 1, &frag_src, NULL);
  glCompileShader(frag);

  #ifdef DEBUG
  r_verify_shader(vert, GL_COMPILE_STATUS);
  r_verify_shader(frag, GL_COMPILE_STATUS);
  #endif

  u32 id = glCreateProgram();\
  glAttachShader(id, frag);
  glAttachShader(id, vert);
  glLinkProgram(id);

  #ifdef DEBUG
  r_verify_shader(id, GL_LINK_STATUS);
  #endif

  glDeleteShader(vert);
  glDeleteShader(frag);

  return (Shader) {id};
}

inline
void r_bind_shader(Shader *shader)
{
  R_ASSERT(glUseProgram(shader->id));
}

inline
void r_unbind_shader(void)
{
  glUseProgram(0);
}

i32 r_set_uniform_1u(Shader *shader, i8 *name, u32 val)
{
  i32 loc = glGetUniformLocation(shader->id, name);
  glUniform1ui(loc, val);
  
  return loc;
}

i32 r_set_uniform_1(Shader *shader, i8 *name, i32 val)
{
  i32 loc = glGetUniformLocation(shader->id, name);
  glUniform1i(loc, val);
  
  return loc;
}

i32 r_set_uniform_1f(Shader *shader, i8 *name, f32 val)
{
  i32 loc = glGetUniformLocation(shader->id, name);
  glUniform1f(loc, val);
  
  return loc;
}

i32 r_set_uniform_2f(Shader *shader, i8 *name, Vec2F vec)
{
  i32 loc = glGetUniformLocation(shader->id, name);
  glUniform2f(loc, vec.x, vec.y);

  return loc;
}

i32 r_set_uniform_3f(Shader *shader, i8 *name, Vec3F vec)
{
  i32 loc = glGetUniformLocation(shader->id, name);
  glUniform3f(loc, vec.x, vec.y, vec.z);
  
  return loc;
}

i32 r_set_uniform_4f(Shader *shader, i8 *name, Vec4F vec)
{
  i32 loc = glGetUniformLocation(shader->id, name);
  glUniform4f(loc, vec.x, vec.y, vec.z, vec.w);
  
  return loc;
}

i32 r_set_uniform_4x4f(Shader *shader, i8 *name, Mat4x4F mat)
{
  i32 loc = glGetUniformLocation(shader->id, name);
  glUniformMatrix4fv(loc, 1, FALSE, &mat.elements[0][0]);
  
  return loc;
}

i32 r_set_uniform_3x3f(Shader *shader, i8 *name, Mat3x3F mat)
{
  i32 loc = glGetUniformLocation(shader->id, name);
  glUniformMatrix3fv(loc, 1, FALSE, &mat.elements[0][0]);
  
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
    i8 log[length];
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

// @Buffer ==================================================================================

Object r_create_vertex_buffer(void *data, u32 size)
{
  u32 id;
  glGenBuffers(1, &id);
  glBindBuffer(GL_ARRAY_BUFFER, id);
  glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
  // glBindBuffer(GL_ARRAY_BUFFER, 0);

  return (Object) {id, 0, 0};
}

inline
void r_bind_vertex_buffer(Object *buffer)
{
  glBindBuffer(GL_ARRAY_BUFFER, buffer->id);
}

inline
void r_unbind_vertex_buffer(void)
{
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Object r_create_index_buffer(void *data, u32 size)
{
  u32 id;
  glGenBuffers(1, &id);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  return (Object) {id, 0, 0};
}

inline
void r_bind_index_buffer(Object *buffer)
{
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->id);
}

inline
void r_unbind_index_buffer(void)
{
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// @VertexArray =============================================================================

Object r_create_vertex_array(u8 attrib_count)
{
  u32 id;
  R_ASSERT(glGenVertexArrays(1, &id));
  R_ASSERT(glBindVertexArray(id));

  return (Object) {id, attrib_count, 0};
}

inline
void r_bind_vertex_array(Object *vertex_array)
{
  R_ASSERT(glBindVertexArray(vertex_array->id));
}

inline
void r_unbind_vertex_array(void)
{
  glBindVertexArray(0);
}

VertexLayout r_create_vertex_layout(Object *v_arr, GLenum type, u32 count)
{
  // u8 type_size = sizeof (typeof (type));
  u8 type_size = 1;
  switch (type)
  {
    case GL_BYTE:  type_size = sizeof (i8);  break;
    case GL_SHORT: type_size = sizeof (i16); break;
    case GL_INT:   type_size = sizeof (i32); break;
    case GL_FLOAT: type_size = sizeof (f32); break;
    default: ASSERT(FALSE);
  }

  VertexLayout layout = 
  {
    .index = v_arr->attrib_index,
    .count = count,
    .data_type = type,
    .normalized = FALSE,
    .stride = count * v_arr->attrib_count * type_size,
    .first = (void *) (u64) (v_arr->attrib_index * count * type_size)
  };

  v_arr->attrib_index++;

  return layout;
}

void r_bind_vertex_layout(VertexLayout *layout)
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

// @Texture2D ===============================================================================

Texture2D r_load_texture2d(const i8 *path)
{
  Texture2D tex = {0};
  glGenTextures(1, &tex.id);
  tex.data = stbi_load(path, &tex.width, &tex.height, &tex.num_channels, 0);

  return tex;
}

inline
void r_bind_texture2d(Texture2D *texture)
{
  glBindTexture(GL_TEXTURE_2D, texture->id);
}

inline
void r_unbind_texture2d(void)
{
  glBindTexture(GL_TEXTURE_2D, 0);
}

void r_gen_textured2(Texture2D *texture)
{
  glTexImage2D(
               GL_TEXTURE_2D, 
               0, 
               GL_RGB, 
               texture->width, 
               texture->height, 
               0, 
               GL_RGB, 
               GL_UNSIGNED_BYTE, 
               texture->data);

  glGenerateMipmap(GL_TEXTURE_2D);
}

// @Draw ====================================================================================

void r_clear(Vec4F color)
{
  glClearColor(color.r, color.g, color.b, color.a);
  glClear(GL_COLOR_BUFFER_BIT);
}

void r_draw(Object *vertex_array, Shader *shader)
{
  r_bind_shader(shader);
  r_bind_vertex_array(vertex_array);
  R_ASSERT(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, NULL));
}
