#include "math.h"
#include <stdio.h>

mat4x4 mat4x4_multiply(mat4x4 *a, mat4x4 *b)
{
	mat4x4 result;

	result.e11 = a->e11 * b->e11 + 
				a->e12 * b->e21 +
				a->e13 * b->e31 +
				a->e14 * b->e41;

	result.e12 = a->e11 * b->e12 + 
				a->e12 * b->e22 +
				a->e13 * b->e32 +
				a->e14 * b->e42;

	result.e13 = a->e11 * b->e13 + 
				a->e12 * b->e23 +
				a->e13 * b->e33 +
				a->e14 * b->e43;

	result.e14 = a->e11 * b->e14 + 
				a->e12 * b->e24 +
				a->e13 * b->e34 +
				a->e14 * b->e44;

	result.e21 = a->e21 * b->e11 + 
				a->e22 * b->e21 +
				a->e23 * b->e31 +
				a->e24 * b->e41;

	result.e22 = a->e21 * b->e12 + 
				a->e22 * b->e22 +
				a->e23 * b->e32 +
				a->e24 * b->e42;

	result.e23 = a->e21 * b->e13 + 
				a->e22 * b->e23 +
				a->e23 * b->e33 +
				a->e24 * b->e43;

	result.e24 = a->e21 * b->e14 + 
				a->e22 * b->e24 +
				a->e23 * b->e34 +
				a->e24 * b->e44;

	result.e31 = a->e31 * b->e11 + 
				a->e32 * b->e21 +
				a->e33 * b->e31 +
				a->e34 * b->e41;

	result.e32 = a->e31 * b->e12 + 
				a->e32 * b->e22 +
				a->e33 * b->e32 +
				a->e34 * b->e42;

	result.e33 = a->e31 * b->e13 + 
				a->e32 * b->e23 +
				a->e33 * b->e33 +
				a->e34 * b->e43;

	result.e34 = a->e31 * b->e14 + 
				a->e32 * b->e24 +
				a->e33 * b->e34 +
				a->e34 * b->e44;

	result.e41 = a->e41 * b->e11 + 
				a->e42 * b->e21 +
				a->e43 * b->e31 +
				a->e44 * b->e41;

	result.e42 = a->e41 * b->e12 + 
				a->e42 * b->e22 +
				a->e43 * b->e32 +
				a->e44 * b->e42;

	result.e43 = a->e41 * b->e13 + 
				a->e42 * b->e23 +
				a->e43 * b->e33 +
				a->e44 * b->e43;

	result.e44 = a->e41 * b->e14 + 
				a->e42 * b->e24 +
				a->e43 * b->e34 +
				a->e44 * b->e44;

	return result;
}

mat4x4 mat4x4_identity()
{
	mat4x4 result =
	{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};
	return result;
}

mat4x4 mat4x4_translation(vector3_f a)
{
	mat4x4 result =
	{
		1, 0, 0, a.x,
		0, 1, 0, a.y,
		0, 0, 1, a.z,
		0, 0, 0, 1
	};
	return result;
}

mat4x4 mat4x4_scaling(vector3_f a)
{
	mat4x4 result =
	{
		a.x, 0, 0, 0,
		0, a.y, 0, 0,
		0, 0, a.z, 0,
		0, 0, 0, 1
	};
	return result;
}

void mat4x4_print(mat4x4* a)
{	
	printf("%f, %f, %f, %f\n%f, %f, %f, %f\n%f, %f, %f, %f\n%f, %f, %f, %f\n", a->e11, a->e12, a->e13, a->e14, a->e21, a->e22, a->e23, a->e24, a->e31, a->e32, a->e33, a->e34, a->e41, a->e42, a->e43, a->e44);
}