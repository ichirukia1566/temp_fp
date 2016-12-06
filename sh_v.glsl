#version 330

layout(location = 0) in vec3 pos;		// Model-space position
layout(location = 1) in vec3 norm;		// Model-space normal
layout(location = 2) in vec3 center;
layout(location = 3) in vec3 color;
layout(location = 4) in int lifetime;
layout(location = 5) in float size;


smooth out vec3 geoNorm;	// Model-space interpolated normal
smooth out vec3 geoCol;
flat out int geoLifetime;
flat out int geoT;


uniform mat4 xform;			// Model-to-clip space transform
uniform int flip;
uniform float speed;
uniform int t;


void main() {
	// Transform vertex position
	
	mat4 translate = mat4(1.0f, 0.0f, 0.0f, 0.0f,
						  0.0f, 1.0f, 0.0f, 0.0f,
						  0.0f, 0.0f, 1.0f, 0.0f,
						  -1.0f * center.x, -1.0f * center.y, -1.0f * center.z, 1.0f);
	mat4 translate1 = mat4(1.0f, 0.0f, 0.0f, 0.0f,
						  0.0f, 1.0f, 0.0f, 0.0f,
						  0.0f, 0.0f, 1.0f, 0.0f,
						  1.0f * center.x, 1.0f * center.y, 1.0f * center.z, 1.0f);
	mat4 move = mat4(1.0f, 0.0f, 0.0f, 0.0f,
					 0.0f, 1.0f, 0.0f, 0.0f,
					 0.0f, 0.0f, 1.0f, 0.0f,
					 speed, 0.0f, 0.0f, 1.0f);
	mat4 rotation = mat4(-1.0f, 0.0f, 0.0f, 0.0f,
						 0.0f, -1.0f, 0.0f, 0.0f,
						 0.0f, 0.0f, 1.0f, 0.0f,
						 0.0f, 0.0f, 0.0f, 1.0f);
	mat4 reflection = mat4(1.0f, 0.0f, 0.0f, 0.0f,
						  0.0f, -1.0f, 0.0f, 0.0f,
						  0.0f, 0.0f, 1.0f, 0.0f,
						  0.0f, 0.0f, 0.0f, 1.0f);
	mat4 scale = mat4(0.01f, 0.0f, 0.0f, 0.0f,
					  0.0f, 0.01f, 0.0f, 0.0f,
					  0.0f, 0.0f, 0.01f, 0.0f,
					  0.0f, 0.0f, 0.0f, 1.0f);
	vec4 temp = translate1 * scale * translate * vec4(pos, 1.0);
	vec4 temp1 = translate1 * scale * reflection * translate * vec4(pos, 1.0);
	if (flip == 0)
	{
		temp = move * temp;
		gl_Position = xform * temp;
	} 
	else
	{
		temp1 = move * temp1;
		gl_Position = xform * temp1;
	}
	

	// Interpolate normals
	geoNorm = norm;
	geoCol = color;
	geoT = t;
	geoLifetime = lifetime;

}