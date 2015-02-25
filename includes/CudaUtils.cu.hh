#ifndef __cuda_utils_cu_hh__
#define __cuda_utils_cu_hh__

#define PI 3.14159

typedef struct
{
    float x;
    float y;
    float z;
} vec3;

typedef struct
{
	vec3 	xyz;
	float	w;
} quaternion;



/*
VECTOR OPERATIONS
*/

__device__
vec3 quaternion2euler(quaternion q);

__device__
void	rx(vec3 &v, float a);
__device__
void	ry(vec3 &v, float a);
__device__
void	rz(vec3 &v, float a);

__device__
float norm(vec3 a);

__device__
vec3 cross(vec3 a, vec3 b);

__device__
quaternion q_normalized(quaternion a);

__device__
vec3 normalized(vec3 a);

__device__
vec3 v_abs(vec3 v);

__device__
float dot(vec3 a, vec3 b);

__device__
vec3 sub(vec3 a, vec3 b);

__device__
vec3 add(vec3 a, vec3 b);

__device__
vec3 mul(vec3 a, float m);

__device__
quaternion getFromToRotation(vec3 a, vec3 b);


__device__
float rad2deg(float rad);

__device__
float deg2rad(float rad);

#endif