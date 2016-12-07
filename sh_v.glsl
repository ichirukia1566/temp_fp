#version 330

layout(location = 0) in vec3 pos;		// Model-space position
layout(location = 1) in vec3 norm;		// Model-space normal
layout(location = 2) in vec3 center;
layout(location = 3) in vec3 color;
layout(location = 4) in int lifetime;
layout(location = 5) in float size;
layout(location = 6) in int maxLifetime;
layout(location = 7) in vec3 curl;


smooth out vec3 geoNorm;	// Model-space interpolated normal
smooth out vec3 geoCol;
flat out int geoLifetime;
flat out int geoT;


uniform mat4 xform;			// Model-to-clip space transform
uniform int flip;
uniform float speed;
uniform int t;

mat4 invX_move(mat4 m)
{
	return mat4(m[0][0], m[0][1], m[0][2], m[0][3],
				m[1][0], m[1][1], m[1][2], m[1][3],
				m[2][0], m[2][1], m[2][2], m[2][3],
				-m[3][0], m[3][1], m[3][2], m[3][3]);
}
mat4 invY_move(mat4 m)
{
	return mat4(m[0][0], m[0][1], m[0][2], m[0][3],
				m[1][0], m[1][1], m[1][2], m[1][3],
				m[2][0], m[2][1], m[2][2], m[2][3],
				m[3][0], -m[3][1], m[3][2], m[3][3]);
}
mat4 invZ_move(mat4 m)
{
	return mat4(m[0][0], m[0][1], m[0][2], m[0][3],
				m[1][0], m[1][1], m[1][2], m[1][3],
				m[2][0], m[2][1], m[2][2], m[2][3],
				m[3][0], m[3][1], -m[3][2], m[3][3]);
}

mat4 scale_move(mat4 m, float f)
{
	return mat4(m[0][0], m[0][1], m[0][2], m[0][3],
				m[1][0], m[1][1], m[1][2], m[1][3],
				m[2][0], m[2][1], m[2][2], m[2][3],
				f * m[3][0],  f * m[3][1], f * m[3][2], m[3][3]);
}

void main() {
	int _t = ((maxLifetime - lifetime) > 0 ) ? (maxLifetime - lifetime) : 0;
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
					 (color.x + 1.0f * curl.x) * -0.01f * t, (color.y + 1.0f * curl.y) * -0.01f * t, (color.z + 1.0f * curl.z) * -0.01f * t, 1.0f);
	//float a = -0.00001f;
	//float vy = 0.0f + a * float(t);
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