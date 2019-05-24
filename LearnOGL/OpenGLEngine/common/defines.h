#pragma once
#include <stdlib.h>
#include <stdint.h>
#include <string>
#include "glad/glad.h"
#include "glm.hpp"
#include "gtc/type_ptr.hpp"
#include "gtc/matrix_transform.hpp"


// Supported color format
typedef enum _color_fmt
{
	COLOR_FMT_UNKNOWN = 0x00,
	COLOR_FMT_GRAY,
	COLOR_FMT_RGB,
	COLOR_FMT_BGR,
	COLOR_FMT_RGBA,
	COLOR_FMT_BGRA
} color_fmt_t;

typedef struct _rect
{
	int top;
	int left;
	int width;
	int height;
} rect_t;

typedef struct _coord_2di
{
	int x;
	int y;
} coord_2di_t;

typedef struct _coord_3di
{
	int x;
	int y;
	int z;
} coord_3di_t;

typedef struct _coord_4di
{
	int x;
	int y;
	int z;
	int w;
} coord_4di_t;

typedef struct _coord_2df
{
	float x;
	float y;
} coord_2df_t;

typedef struct _coord_3df
{
	float x;
	float y;
	float z;
} coord_3df_t;

typedef struct _coord_4df
{
	float x;
	float y;
	float z;
	float w;
} coord_4df_t;

