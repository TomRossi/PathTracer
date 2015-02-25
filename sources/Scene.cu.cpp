#include "Scene.cu.hh"

void	setScene(Scene* h_scene, float x, float y, float z){
	int n_objects 	= 4;
	int n_lights 	= 1;

	h_scene->camera.position.x = x;
	h_scene->camera.position.y = y;
	h_scene->camera.position.z = z;

	h_scene->objects_size = n_objects;
	h_scene->lights_size = n_lights;

	h_scene->objects = (Object*)malloc(sizeof(Object) * h_scene->objects_size);
	h_scene->lights = (Light*)malloc(sizeof(Light) * h_scene->lights_size);


	Material m;
	Object obj;

	m.color.x = 1;
	m.color.y = 0;
	m.color.z = 1;
	m.diffuse = 0.9;
	m.specular = 1.7;

	obj.type = 0x0;
	obj.position.x = 1.5;
	obj.position.y = 1;
	obj.position.z = 2.5;
	obj.material = m;
	obj.size = 1;
	obj.material.reflection = 0;

	h_scene->objects[0] = obj;

	obj.position.x = -1;
	obj.position.y = 2;
	obj.position.z = -1;

	obj.size = 2;

	obj.material.color.x = .5;
	obj.material.color.y = .5;
	obj.material.color.z = .5;
	obj.material.diffuse = 1;
	obj.material.specular = .1;
	obj.material.reflection = .8;
	
	h_scene->objects[1] = obj;

	obj.type = 0x1;
	obj.position.x = 0;
	obj.position.y = 0;
	obj.position.z = 0;

	obj.material.color.x = .4;
	obj.material.color.y = .4;
	obj.material.color.z = .4;
	obj.material.diffuse = 1;
	obj.material.specular = .5;
	obj.material.reflection = 0.2;

	h_scene->objects[2] = obj;


	obj.type = 0x0;
	obj.position.x = -1;
	obj.position.y = 3;
	obj.position.z = 6;
	obj.size = 3;

	obj.material.color.x = .4;
	obj.material.color.y = .4;
	obj.material.color.z = .4;
	obj.material.diffuse = 1;
	obj.material.specular = .5;
	obj.material.reflection = 0.8;

	h_scene->objects[3] = obj;

		Light light;
	light.position.x = 5;
	light.position.y = 6;
	light.position.z = -3;
	light.intensity = 1;
	light.type = 0x0;

	h_scene->lights[0] = light;

	light.position.x = 2;
	light.position.y = 8;
	light.position.z = 4;
	//h_scene->lights[1] = light;
}