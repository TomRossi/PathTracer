#include "CudaUtils.cu.hh"

__device__
vec3 quaternion2euler(quaternion q){
	float w,x,y,z;
	vec3 euler;

    w = q.w;
    x = q.xyz.x;
    y = q.xyz.y;
    z = q.xyz.z;

    double sqw = w*w;    
    double sqx = x*x;    
    double sqy = y*y;    
    double sqz = z*z; 

    euler.z = (atan2(2.0 * (x*y + z*w),(sqx - sqy - sqz + sqw)) * (180.0f / PI));
    euler.x = (atan2(2.0 * (y*z + x*w),(-sqx - sqy + sqz + sqw)) * (180.0f / PI));          
    euler.y = (asin(-2.0 * (x*z - y*w)) * (180.0f / PI));
    return euler;
}

__device__
void	rx(vec3 &v, float a)
{
  float sy = v.y;
  float sz = v.z;

  v.y = cos(a) * sy - sin(a) * sz;
  v.z = sin(a) * sy + cos(a) * sz;
}

__device__
void	ry(vec3 &v, float a)
{
  float sx = v.x;
  float sz = v.z;

  v.x = cos(a) * sx + sin(a) * sz;
  v.z = -sin(a) * sx + cos(a) * sz;
}

__device__
void	rz(vec3 &v, float a)
{
  float sx = v.x;
  float sy = v.y;

  v.x = cos(a) * sx - sin(a) * sy;
  v.y = sin(a) * sx + cos(a) * sy;
}

__device__
float norm(vec3 a){
  return sqrtf(a.x * a.x + a.y * a.y + a.z * a.z);
}

__device__
vec3 cross(vec3 a, vec3 b){
  vec3 out = {.x = a.y * b.z - a.z * b.y,
        .y = a.z * b.x - a.x * b.z,
        .z = a.x * b.y - a.y * b.x};
  return out;
}

__device__
quaternion q_normalized(quaternion a){
  quaternion q = a;
  float n = sqrtf(q.xyz.x * q.xyz.x +
          q.xyz.y * q.xyz.y + 
          q.xyz.z * q.xyz.z +
          q.w * q.w);
  q.xyz.x /= n;
  q.xyz.y /= n;
  q.xyz.z /= n;
  q.w /= n;
  return q;
}

__device__
vec3 normalized(vec3 a){
  vec3 out;
  float n = norm(a);
  
  out.x = a.x / n;
  out.y = a.y / n;
  out.z = a.z / n;
  return out;
}

__device__
vec3 v_abs(vec3 v){
  vec3 a;

  a.x = abs(v.x);
  a.y = abs(v.y);
  a.z = abs(v.z);
  return a;
}

__device__
float dot(vec3 a, vec3 b){
  return a.x * b.x + a.y * b.y + a.z * b.z;
}

__device__
vec3 sub(vec3 a, vec3 b){
  vec3 o = {.x = a.x - b.x, .y = a.y - b.y, .z = a.z - b.z};
  return o;
}

__device__
vec3 add(vec3 a, vec3 b){
  vec3 o = {.x = a.x + b.x, .y = a.y + b.y, .z = a.z + b.z};
  return o;
}

__device__
vec3 mul(vec3 a, float m){
  vec3 o = {.x = a.x * m, .y = a.y * m, .z = a.z * m};
  return o;
}


__device__
quaternion getFromToRotation(vec3 a, vec3 b){
  quaternion rotation;

  rotation.xyz = cross(a, b);
  rotation.w = sqrtf((norm(b) * norm(b))
            * (norm(a) * norm(a)))
        + dot(a, b);
  rotation = q_normalized(rotation);
  return rotation;
}

__device__
float rad2deg(float rad){
  return rad * 57.295;
}

__device__
float deg2rad(float rad){
  return rad / 57.295;
}