#version 330 

layout (location = 0) in vec3 center; 
layout(location = 1) in vec3 color;

layout(location = 3) in vec3 curl;
layout(location = 4) in float size;
layout(location = 5) in float lifetime;
layout(location = 6) in int type;

out vec3 vColorPass; 
out float fLifeTimePass; 
out float fSizePass; 
out int iTypePass; 

void main() 
{ 
   gl_Position = vec4(center, 1.0); 
   vColorPass = color; 
   fSizePass = size; 
   fLifeTimePass = lifetime;
   iTypePass = type;
}