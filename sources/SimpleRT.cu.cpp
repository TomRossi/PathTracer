#include <cuda.h>
#include <curand.h>
#include <curand_kernel.h>
#include <stdio.h>

#include "Image.hh"
#include "Scene.cu.hh"

__device__
bool sphereIntersect(vec3 c_p, vec3 c_d, vec3 sp, float radius, out& data){
	vec3 d = sub(c_p, sp);

	float a = dot(c_d, c_d);
	float b = dot(c_d, d);
	float c = dot(d, d) - radius * radius;
	float t = b * b - c;
	if (t > 0){
        t = -b - sqrt(t);

        data.point = add(mul(c_d, t), c_p);
        data.norm = normalized(sub(data.point, sp));
        data.distance = t;
        return t > 0;
	}
	return false;
}

__device__
bool planeIntersect(vec3 c_p, vec3 c_d, vec3 sp, out& data){
	vec3 d = sub(c_p, sp);
	vec3 up = {.x = 0, .y = 1, .z = 0};
	float t = -dot(c_p, up) / dot(c_d, up);
	if (t > 0){
		data.distance = t;
		data.norm = up;
		data.point = add(c_p, mul(c_d, t));
		return true;
	}
	return false;
}


__device__
float angle(vec3 a, vec3 b){
	float d = dot(a, b);
	float e = norm(a) * norm(b);
	return acos(d / e);
}

__device__
out	getIntersect(Scene& s, vec3 ro, vec3 rd){
	ro = add(ro, mul(rd, 0.01)); //quick fix
	out hit_data;
	float last_hit = -1;
	out last_hit_data;

	last_hit_data.idx = -1;

	for (int i = 0;i < s.objects_size;++i){
		Object obj = s.objects[i];
		bool hit = false;
		if (obj.type == 0x0)
			hit = sphereIntersect(ro, rd, obj.position, obj.size, hit_data);
		else
			hit = planeIntersect(ro, rd, obj.position, hit_data);
		if (hit && (hit_data.distance < last_hit || last_hit < 0)){
			hit_data.idx = i;
			last_hit_data = hit_data;
			last_hit = hit_data.distance;
		}
	}
	last_hit_data.ro = ro;
	last_hit_data.rd = rd;
	return  last_hit_data;
}

__device__
float	getLight(Scene s, out data){
	float l = 0;
	for (int i = 0;i < s.lights_size;++i){
		
		vec3 dif = sub(s.lights[i].position, data.point);
		vec3 dir = normalized(dif);
		float dis = norm(dif);

		out hit_data;
		hit_data = getIntersect(s, add(data.point,
			mul(normalized(dir), 0.01)), dir); //so it deosnt intersect with itself
		if (hit_data.idx < 0){
			vec3 reflectionVector = add(mul(data.norm, -2 * dot(dir, data.norm)), dir);
			vec3 viewingDirection = sub(data.point, s.camera.position);
			viewingDirection = normalized(viewingDirection);
			float val = dot(viewingDirection, reflectionVector);
			val = min(1.0, max(0.0, val));

			float specularLighting = pow(val, 16);

			l += max(0.0, dot(dir, data.norm)) *
			 	s.objects[data.idx].material.diffuse / (float)s.lights_size;
			l += specularLighting * s.objects[data.idx].material.specular;
		}
	}
	return l;
}

__device__
vec3 getRayDirection(float x, float y, int width, vec3 position){
	vec3 c_d;

	c_d.x = 2.0f * x / (float)width - 1.0f;
	c_d.y = (2.0f * y) / (float)width - 1.0f;
	c_d.z = 1;

	vec3 zero 		= {.x = 0, .y = 0, .z = 0};
	vec3 forward 	= {.x = 0, .y = 0, .z = 1};
	vec3 direction 	= normalized(sub(zero, position));


	quaternion rotation = getFromToRotation(forward, direction);
	vec3 euler = quaternion2euler(rotation);
	
	c_d = normalized(c_d);
	if (position.z < 0)
		euler.z *= -1;
	rz(c_d, deg2rad(euler.z));
	ry(c_d, deg2rad(euler.y));
	rx(c_d, deg2rad(euler.x));
	return c_d;
}

__device__
vec3 applyColor(Scene& s, Material& mat, out& hit){
	vec3 color;

	color.x = 50;
	color.y = 80;
	color.z = 100;	
	float l = max(0.1, getLight(s, hit));
	float zero = 0.1;
	color.x = min(255.0, l * (mat.color.x + zero) * 255.0);
	color.y = min(255.0, l * (mat.color.y + zero) * 255.0);
	color.z = min(255.0, l * (mat.color.z + zero) * 255.0);
	return color;
}

#define RECURSION 4

__device__
vec3 getColor(Scene& s, vec3 c_p, vec3 c_d){
	vec3 bg = {.x = 50, .y = 80, .z = 100};
	vec3 color = {.x = 0, .y = 0, .z = 0};

	out	array[RECURSION];

	array[0] = getIntersect(s, c_p, c_d);
	if (array[0].idx < 0)
		return bg;
	for (int i = 1;i < RECURSION;i++){
		vec3 reflectionVector;

		out hit;
		hit.idx = -1;
		if (array[i - 1].idx >= 0){
			reflectionVector = add(mul(array[i - 1].norm,
				-2 * dot(array[i - 1].rd, array[i - 1].norm)), array[i - 1].rd);
			//reflectionVector = mul(reflectionVector, 2);
			hit = getIntersect(s, array[i - 1].point, reflectionVector);
		}
		array[i] = hit;
	}

	int i = RECURSION - 1;
	while (i > 0 && array[i].idx < 0)
		i--;

	color = bg;
	vec3 newColor;
	while (i >= 0){
		out hit = array[i];
		newColor = applyColor(s, s.objects[hit.idx].material, hit);
		color = mul(color, s.objects[hit.idx].material.reflection);
		newColor = mul(newColor, 1 - s.objects[hit.idx].material.reflection);	
		color = add(color, newColor);
		i--;
	}
	return color;
}

__global__
void setup_kernel(curandState * state, unsigned long seed, int width)
{
	int x = threadIdx.x + blockIdx.x * blockDim.x;
  	int y = threadIdx.y + blockIdx.y * blockDim.y;
	int id = x + y * width;
	curand_init (seed, id, 0, &state[id]);
} 



__device__
float generate(curandState* globalState, int idx) 
{

    curandState localState = globalState[idx];
    float RANDOM = curand_uniform( &localState );
    globalState[idx] = localState; 
    return RANDOM;
}

__global__
void toast(unsigned char* data, int width, Scene s, curandState* globalState)
{
	vec3 color = {};
	int x = threadIdx.x + blockIdx.x * blockDim.x;
  	int y = threadIdx.y + blockIdx.y * blockDim.y;
  	int iterations = 1;
	int idx = x + y * width;
	idx *= 3;

	vec3 c_p;
	c_p.x = s.camera.position.x;
	c_p.y = s.camera.position.y;
	c_p.z = s.camera.position.z;

	for (int i = 0;i < iterations;++i){
		float rx = generate(globalState, idx / 3) - 0.5;
		float ry = generate(globalState, idx / 3) - 0.5;

		vec3 c_d = getRayDirection(x + rx, y + ry, width, c_p);
		
		color = add(color, getColor(s, c_p, c_d));
	}

	
	data[idx] = color.x / iterations;
	data[idx + 1] = color.y / iterations;
	data[idx + 2] = color.z / iterations;

}



void copy_scene_to_device(Scene* h_scene, Scene* d_scene){
	*d_scene = *h_scene;

	Object* objects;
	cudaMalloc((void**)&objects, sizeof(Object) * h_scene->objects_size * sizeof(Object));
	cudaMemcpy(objects, h_scene->objects,
		h_scene->objects_size * sizeof(Object), cudaMemcpyHostToDevice);
	d_scene->objects = objects;

	Light* lights;
	cudaMalloc((void**)&lights, sizeof(Light) * h_scene->lights_size * sizeof(Light));
	cudaMemcpy(lights, h_scene->lights,
		h_scene->lights_size * sizeof(Light), cudaMemcpyHostToDevice);
	d_scene->lights = lights;
}

void trace(Image* image, float x, float y, float z){
	int length = image->getLength();

	unsigned char* h_string = (unsigned char*)malloc(length);
  	unsigned char* d_string;
  	cudaMalloc((void**)&d_string, length);
	cudaMemcpy(d_string, h_string, length, cudaMemcpyHostToDevice);

	/* random */
	static curandState* devStates = NULL;

	if (devStates == NULL)cudaMalloc (&devStates, length * sizeof(curandState));
    



	int THREADS = 16;

	dim3 dimBlock(THREADS, THREADS);
	dim3 dimGrid(image->getWidth() / THREADS, image->getHeight() / THREADS);
	
	static bool t = false;
	if (!t)
		setup_kernel<<<dimGrid, dimBlock>>>(devStates, time(NULL), image->getWidth());
	t = true;
	Scene h_scene;
	Scene d_scene;

	setScene(&h_scene, x, y, z);

	copy_scene_to_device(&h_scene, &d_scene);
	toast<<<dimGrid, dimBlock>>>(d_string, image->getWidth(), d_scene, devStates);

	cudaMemcpy(h_string, d_string, length, cudaMemcpyDeviceToHost);

	image->setBuffer(h_string, image->getLength());
}