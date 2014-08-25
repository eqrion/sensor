#include "math.h"

mat4x4 mat4x4_identity()
{
	mat4x4 result;
	result.entry[0][0] = 1;
	result.entry[0][1] = 0;
	result.entry[0][2] = 0;
	result.entry[0][3] = 0;

	result.entry[1][0] = 0;
	result.entry[1][1] = 1;
	result.entry[1][2] = 0;
	result.entry[1][3] = 0;

	result.entry[2][0] = 0;
	result.entry[2][1] = 0;
	result.entry[2][2] = 1;
	result.entry[2][3] = 0;

	result.entry[3][0] = 0;
	result.entry[3][1] = 0;
	result.entry[3][2] = 0;
	result.entry[3][3] = 1;
	return result;
}