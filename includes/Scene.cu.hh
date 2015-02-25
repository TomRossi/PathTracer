#ifndef __scene_h__
#define __scene_h__

#include "CudaUtils.cu.hh"

typedef struct{
	vec3 	position;
	float	fov;
	float	aperture;
	float	shutter;
} Camera;

typedef struct{
	vec3	color;
	float	diffuse;
	float	specular;
	float	reflection; //0-1
} Material;

#define OBJ_SPHERE 	= 0x0;
#define OBJ_PLAN	= 0x1;

typedef struct{
	char		type;
	vec3		position;
	float		size;
	Material	material;
} Object;

typedef struct
{
	int 	idx;
	vec3	point;
	vec3 	norm;
	float	distance;
	vec3	ro;
	vec3	rd;
} out;

#define LIGHT_POINT			= 0x0;
#define LIGHT_DIRECTIONAL 	= 0x01;
#define LIGHT_SPOT			= 0x02;

typedef struct{
	char		type;
	vec3		position;
	vec3		color;
	float		intensity;
} Light;

typedef struct{
	Camera	camera;

	Object*	objects;
	int		objects_size;

	Light*	lights;
	int		lights_size;
} Scene;

void	setScene(Scene* h_scene, float x, float y, float z);


#endif