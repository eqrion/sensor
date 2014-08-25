#ifndef sensor_math_h
#define sensor_math_h

#include <stdint.h>

typedef float real;
typedef int32_t int_m;
typedef uint32_t uint_m;

#define CLAMP_F(a, min, max) fmin(fmax(a, min), max)

typedef struct
{
	union
	{
		real entry[4][4];
		struct
		{
			real e11;
			real e12;
			real e13;
			real e14;
			real e21;
			real e22;
			real e23;
			real e24;
			real e31;
			real e32;
			real e33;
			real e34;
			real e41;
			real e42;
			real e43;
			real e44;
		};
	};
} mat4x4;

typedef struct
{
	real x, y;
} vector2_f;
typedef struct
{
	int x, y;
} vector2_i;

typedef struct
{
	real x, y, z;
} vector3_f;
typedef struct
{
	int x, y, z;
} vector3_i;

typedef struct
{
	real x, y, z, w;
} vector4_f;
typedef struct
{
	int x, y, z, w;
} vector4_i;

mat4x4 mat4x4_identity();

#endif