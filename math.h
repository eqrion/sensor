#ifndef sensor_math_h
#define sensor_math_h

#include <stdint.h>

#define M_2_PI 6.28318530717958647692528676655900576839433879875021164194989
#define M_3_OVER_2_PI 4.71238898038468985769396507491925432629575409906265873146242
#define M_PI 3.14159265358979323846264338327950288419716939937510582097494
#define M_PI_OVER_2 1.57079632679489661923132169163975144209858469968755291048747
#define M_PI_OVER_4 0.785398163397448309615660845819875721049292349843776455243736
#define M_PI_OVER_6 0.523598775598298873077107230546583814032861566562517636829157
#define M_PI_OVER_8 0.392699081698724154807830422909937860524646174921888227621868

typedef float real;
typedef int_fast32_t int_m;
typedef uint_fast32_t uint_m;

#define CLAMP_F(a, min, max) fmin(fmax(a, min), max)

typedef struct
{
	real x, y;
} vector2_f;
typedef struct
{
	int_m x, y;
} vector2_i;

vector2_f vec2_scale(vector2_f a, real b);
vector2_f vec2_add(vector2_f a, vector2_f b);

typedef struct
{
	real x, y, z;
} vector3_f;
typedef struct
{
	int_m x, y, z;
} vector3_i;

vector3_f vec3_scale(vector3_f a, real b);
vector3_f vec3_scale_inv(vector3_f a, real b);
vector3_f vec3_add(vector3_f a, vector3_f b);
vector3_f vec3_subtract(vector3_f a, vector3_f b);
real vec3_dot(vector3_f a, vector3_f b);
vector3_f vec3_cross(vector3_f a, vector3_f b);
vector3_f vec3_normalize(vector3_f a);
real vec3_length(vector3_f a);

vector3_f convert_to_barycentric2D(vector3_f euclidean_point, vector3_f tri1, vector3_f tri2, vector3_f tri3);
vector3_f convert_to_barycentric3D(vector3_f euclidean_point, vector3_f tri1, vector3_f tri2, vector3_f tri3);

typedef struct
{
	real x, y, z, w;
} vector4_f;
typedef struct
{
	int_m x, y, z, w;
} vector4_i;

vector4_f vec4_scale(vector4_f a, real b);
vector4_f vec4_add(vector4_f a, vector4_f b);

typedef struct
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
} mat4x4;

mat4x4 mat4x4_multiply(mat4x4 *a, mat4x4 *b);
vector3_f mat4x4_transform(mat4x4 *a, vector3_f* vector);
void mat4x4_transform_stream(mat4x4 *a, vector3_f* vector, uint_m vector_count);

mat4x4 mat4x4_identity();
mat4x4 mat4x4_translation(vector3_f a);
mat4x4 mat4x4_scaling(vector3_f a);

mat4x4 mat4x4_rot_x(real theta);
mat4x4 mat4x4_rot_y(real theta);
mat4x4 mat4x4_rot_z(real theta);

mat4x4 mat4x4_look_at_rh(vector3_f from, vector3_f to, vector3_f up_direction);
mat4x4 mat4x4_projection_fov_rh(real l, real r, real b, real t, real n, real f);

void mat4x4_print(mat4x4 *a);

#endif