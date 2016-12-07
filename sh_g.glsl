#version 330

layout(triangles) in;
layout(triangle_strip, max_vertices=3) out;


in vec3 geoNorm[];
in vec3 geoCol[];
flat in int geoLifetime[];
flat in int geoT[];

smooth out vec3 fragPos;
smooth out vec3 fragNorm;
smooth out vec3 fragCol;

void main()
{
	if (geoLifetime[0] >= 0)
	{
		gl_Position = gl_in[0].gl_Position;
		fragPos = (gl_in[0].gl_Position).xyz;
		fragNorm = geoNorm[0];
		fragCol = geoCol[0];
		EmitVertex();

		gl_Position = gl_in[1].gl_Position;
		fragPos = (gl_in[1].gl_Position).xyz;
		fragNorm = geoNorm[1];
		fragCol = geoCol[1];
		EmitVertex();

		gl_Position = gl_in[2].gl_Position;
		fragPos = (gl_in[2].gl_Position).xyz;
		fragNorm = geoNorm[2];
		fragCol = geoCol[2];
		EmitVertex();
		EndPrimitive();
	}
	else
	{
		// do nothing
	}
}