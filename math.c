#include "math.h"
#include <stdio.h>
#include <math.h>

vector2_f vec2_scale(vector2_f a, real b)
{
	vector2_f result = {
		a.x * b,
		a.y * b
	};
	return result;
}
vector2_f vec2_add(vector2_f a, vector2_f b)
{
	vector2_f result = {
		a.x + b.x,
		a.y + b.y
	};
	return result;	
}

vector3_f vec3_scale(vector3_f a, real b)
{
	vector3_f result = {
		a.x * b,
		a.y * b,
		a.z * b
	};
	return result;
}
vector3_f vec3_scale_inv(vector3_f a, real b)
{
	vector3_f result = {
		a.x / b,
		a.y / b,
		a.z / b
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
vector3_f vec3_subtract(vector3_f a, vector3_f b)
{
	vector3_f result = {
		a.x - b.x,
		a.y - b.y,
		a.z - b.z
	};
	return result;	
}
real vec3_dot(vector3_f a, vector3_f b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}
vector3_f vec3_cross(vector3_f a, vector3_f b)
{
	vector3_f result = {
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x,
	};
	return result;
}
vector3_f vec3_normalize(vector3_f a)
{
	real length = vec3_length(a);
	vec3_scale_inv(a, length);
	return a;
}
real vec3_length(vector3_f a)
{
	return sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
}

vector3_f convert_to_barycentric2D(vector3_f euclidean_point, vector3_f tri1, vector3_f tri2, vector3_f tri3)
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
vector3_f convert_to_barycentric3D(vector3_f euclidean_point, vector3_f tri1, vector3_f tri2, vector3_f tri3)
{
	vector3_f result;

	real determinant =
		-tri3.x * tri2.y * tri1.z +
		tri2.x * tri3.y * tri1.z +
		tri3.x * tri1.y * tri2.z -
		tri1.x * tri3.y * tri2.z -
		tri2.x * tri1.y * tri3.z +
		tri1.x * tri2.y * tri3.z;

	result.x = euclidean_point.x * (-tri3.y * tri2.z + tri2.y * tri3.z) + euclidean_point.y * (tri3.x * tri2.z - tri2.x * tri3.z) + euclidean_point.z * (-tri3.x * tri2.y + tri2.x * tri3.y);
	result.y = euclidean_point.x * (tri3.y * tri1.z - tri1.y * tri3.z) + euclidean_point.y * (-tri3.x * tri1.z + tri1.x * tri3.z) + euclidean_point.z * (tri3.x * tri1.y - tri1.x * tri3.y);
	result.z = euclidean_point.x * (-tri2.y * tri1.z + tri1.y * tri2.z) + euclidean_point.y * (tri2.x * tri1.z - tri1.x * tri2.z) + euclidean_point.z * (-tri2.x * tri1.y + tri1.x * tri2.y);

	result.x /= determinant;
	result.y /= determinant;
	result.z /= determinant;

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
	real w = a->e41 * vector->x + a->e42 * vector->y + a->e43 * vector->z + a->e44;
	
	result.x /= w;
	result.y /= w;
	result.z /= w;

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

mat4x4 mat4x4_look_at_rh(vector3_f from, vector3_f to, vector3_f up_direction)
{
	/*
	zaxis = normal(cameraPosition - cameraTarget)
	xaxis = normal(cross(cameraUpVector, zaxis))
	yaxis = cross(zaxis, xaxis)

	 xaxis.x           yaxis.x           zaxis.x          0
	 xaxis.y           yaxis.y           zaxis.y          0
	 xaxis.z           yaxis.z           zaxis.z          0
	-dot(xaxis, cameraPosition)  -dot(yaxis, cameraPosition)  -dot(zaxis, cameraPosition)  1
	*/

	vector3_f zaxis = vec3_normalize(vec3_subtract(from, to));
	vector3_f xaxis = vec3_normalize(vec3_cross(up_direction, zaxis));
	vector3_f yaxis = vec3_cross(zaxis, xaxis);

	mat4x4 result = {
		xaxis.x, xaxis.y, xaxis.z, -vec3_dot(xaxis, from),
		yaxis.x, yaxis.y, yaxis.z, -vec3_dot(yaxis, from),
		zaxis.x, zaxis.y, zaxis.z, -vec3_dot(zaxis, from),
		0, 0, 0, 1
	};
	return result;
}

mat4x4 mat4x4_projection_fov_rh(real l, real r, real b, real t, real n, real f)
{
	mat4x4 result = {
		(2 * n) / (r - l), 	0,					-(r + l)/(r - l),	0,
		0, 					(2 * n) / (t - b), 	-(t + b)/(t - b),	0,
		0, 					0, 					(f + n) / (f - n),	-(2 * f * n) / (f - n),
		0, 					0,					1,					0
	};
	return result;
}

void mat4x4_print(mat4x4* a)
{	
	printf("%f, %f, %f, %f\n%f, %f, %f, %f\n%f, %f, %f, %f\n%f, %f, %f, %f\n",
		a->e11, a->e12, a->e13, a->e14,
		a->e21, a->e22, a->e23, a->e24,
		a->e31, a->e32, a->e33, a->e34,
		a->e41, a->e42, a->e43, a->e44
		);
}