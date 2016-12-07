#version 330

layout(location = 0) in vec3 center;
layout(location = 1) in vec3 color;
layout(location = 2) in vec3 velocity;
layout(location = 3) in vec3 curl;
layout(location = 4) in float size;
layout(location = 5) in float lifetime;
layout(location = 6) in int type;

smooth out vec3 vCenterPass;
smooth out vec3 vColorPass;
smooth out vec3 vVelocityPass;
smooth out vec3 vCurlPass;
out float fSizePass;
out float fLifetimePass;
flat out int iTypePass;

void main() 
{
	vCenterPass = center;
	vColorPass = color;
	vVelocityPass = velocity;
	vCurlPass = curl;
	fSizePass = size;
	fLifetimePass = lifetime;
	iTypePass = type;
}