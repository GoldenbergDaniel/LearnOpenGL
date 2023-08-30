#pragma once

#include <assert.h>

typedef unsigned char bool;
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long u64;
typedef char i8;
typedef short i16;
typedef int i32;
typedef long i64;
typedef float f32;
typedef double f64;

#define FALSE 0
#define TRUE 1

#define ASSERT(exp) assert(exp)
