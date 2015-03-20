#include "math.h"
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>

vector3_f vec3_scale(vector3_f a, real b)
{
	vector3_f result = {
		a.x * b,
		a.y * b,
		a.z * b
	};
	return result;
}
vector3_f vec3_add(vector3_f a, vector3_f b)
{
	vector3_f result = {
		a.x + b.x,
		a.y + b.y,
		a.z + b.z
	};
	return result;	
}

vector4_f vec4_scale(vector4_f a, real b)
{
	vector4_f result = {
		a.x * b,
		a.y * b,
		a.z * b,
		a.w * b
	};
	return result;
}
vector4_f vec4_add(vector4_f a, vector4_f b)
{
	vector4_f result = {
		a.x + b.x,
		a.y + b.y,
		a.z + b.z,
		a.w + b.w
	};
	return result;
}

vector3_f convert_to_barycentric(vector3_f euclidean_point, vector3_f tri1, vector3_f tri2, vector3_f tri3)
{
	vector3_f result;

	real determinant =
		(tri2.y - tri3.y) * (tri1.x - tri3.x) + 
		(tri3.x - tri2.x) * (tri1.y - tri3.y);

	result.x = (
		(tri2.y - tri3.y) * (euclidean_point.x - tri3.x) + 
		(tri3.x - tri2.x) * (euclidean_point.y - tri3.y)
		) / determinant;
	result.y = (
		(tri3.y - tri1.y) * (euclidean_point.x - tri3.x) + 
		(tri1.x - tri3.x) * (euclidean_point.y - tri3.y)
		) / determinant;
	result.z = 1 - result.x - result.y;

	return result;
}

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
vector3_f mat4x4_transform(mat4x4 *a, vector3_f* vector)
{
	/*{
	{e14 + e11 x + e12 y + e13 z},
	{e24 + e21 x + e22 y + e23 z},
	{e34 + e31 x + e32 y + e33 z},
	{e44 + e41 x + e42 y + e43 z}
	}*/

	vector3_f result = 
	{
		a->e11 * vector->x + a->e12 * vector->y + a->e13 * vector->z + a->e14,
		a->e21 * vector->x + a->e22 * vector->y + a->e23 * vector->z + a->e24,
		a->e31 * vector->x + a->e32 * vector->y + a->e33 * vector->z + a->e34,
	};
	return result;
}
void mat4x4_transform_stream(mat4x4 *a, vector3_f* vector, uint_m vector_count)
{
	while (vector_count > 0)
	{
		(*vector) = mat4x4_transform(a, vector);
		++vector;
		--vector_count;
	}
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

mat4x4 mat4x4_rot_x(real theta)
{
	real cos_theta = cos(theta);
	real sin_theta = sin(theta);

	mat4x4 result =
	{
		1, 0, 0, 0,
		0, cos_theta, -sin_theta, 0,
		0, sin_theta, cos_theta, 0,
		0, 0, 0, 1
	};
	return result;	
}
mat4x4 mat4x4_rot_y(real theta)
{
	real cos_theta = cos(theta);
	real sin_theta = sin(theta);

	mat4x4 result =
	{
		cos_theta, 0, sin_theta, 0,
		0, 1, 0, 0,
		-sin_theta, 0, cos_theta, 0,
		0, 0, 0, 1
	};
	return result;	
}
mat4x4 mat4x4_rot_z(real theta)
{
	real cos_theta = cos(theta);
	real sin_theta = sin(theta);

	mat4x4 result =
	{
		cos_theta, -sin_theta, 0, 0,
		sin_theta, cos_theta, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};
	return result;	
}

void mat4x4_print(mat4x4* a)
{	
	printf("%f, %f, %f, %f\n%f, %f, %f, %f\n%f, %f, %f, %f\n%f, %f, %f, %f\n", a->e11, a->e12, a->e13, a->e14, a->e21, a->e22, a->e23, a->e24, a->e31, a->e32, a->e33, a->e34, a->e41, a->e42, a->e43, a->e44);
}