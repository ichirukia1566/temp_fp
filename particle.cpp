#include "particle.h"

using namespace std;
using namespace glm;

Particle::Particle(int l, vec3 c, float s, float r, vec3 col)
{
	lifetime = l;
	center = c;
	size = s;
	rotation = r;
	color = col;
	mat4 temp_rotation = mat4(cos(r), 0.0f, -sin(r), 0.0f,
							  0.0f, 1.0f, 0.0f, 0.0f,
							  sin(r), 0.0f, cos(r), 0.0f,
							  0.0f, 0.0f, 0.0f, 1.0f);
	vertices[0] = c + vec3(0.0f, sqrt(3) * s / 3.0f, 0.0f);
	vertices[1] = vec3(temp_rotation * vec4(c + vec3(-s * 0.5f, -sqrt(3) * s / 6.0f, 0.0f), 1.0f));
	vertices[2] = vec3(temp_rotation * vec4(c + vec3(s * 0.5f, -sqrt(3) * s / 6.0f, 0.0f), 1.0f));
	norm[0] = normalize(vec3(temp_rotation * vec4(vertices[0].x, vertices[0].y, 1.0f, 0.0f)));
	norm[1] = normalize(vec3(temp_rotation * vec4(vertices[1].x, vertices[1].y, 1.0f, 0.0f)));
	norm[2] = normalize(vec3(temp_rotation * vec4(vertices[2].x, vertices[2].y, 1.0f, 0.0f)));
}

vec3* Particle::getVertices()
{
	return vertices;
}

vec3 Particle::getColor()
{
	return color;
}

vec3* Particle::getNorm()
{
	return norm;
}

int Particle::getLifetime()
{
	return lifetime;
}

float Particle::getSize()
{
	return size;
}

float Particle::getRotation()
{
	return rotation;
}

vec3 Particle::getCenter()
{
	return center;
}

void Particle::setCenter(vec3 newCenter)
{
	mat4 temp_rotation = mat4(cos(rotation), 0.0f, -sin(rotation), 0.0f,
							  0.0f, 1.0f, 0.0f, 0.0f,
							  sin(rotation), 0.0f, cos(rotation), 0.0f,
							  0.0f, 0.0f, 0.0f, 1.0f);
	for (int i = 0; i < 3; i++)
	{
		vertices[i] += (newCenter - center);
		norm[i] = normalize(vec3(temp_rotation * vec4(vertices[i].x, vertices[i].y, 1.0f, 0.0f)));
	}
	center = newCenter;
}
void Particle::setSize(float newSize)
{
	float scale = newSize / size;
	mat4 temp_rotation = mat4(cos(rotation), 0.0f, -sin(rotation), 0.0f,
							  0.0f, 1.0f, 0.0f, 0.0f,
							  sin(rotation), 0.0f, cos(rotation), 0.0f,
							  0.0f, 0.0f, 0.0f, 1.0f);
	for (int i = 0; i < 3; i++)
	{
		vertices[i] = vertices[i] + (scale - 1.0f) * (vertices[i] - center);
		norm[i] = normalize(vec3(temp_rotation * vec4(vertices[i].x, vertices[i].y, 1.0f, 0.0f)));
	}
	size = newSize;
}
void Particle::setRotation(float newRotation)
{
	float diff = newRotation - rotation;
	mat4 temp_rotation = mat4(cos(diff), 0.0f, -sin(diff), 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		sin(diff), 0.0f, cos(diff), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
	for (int i = 0; i < 3; i++)
	{
		vertices[i] = vec3(temp_rotation * vec4(vertices[i], 1.0f));
		norm[i] = normalize(temp_rotation * vec4(norm[i], 0.0f));
	}
	rotation = newRotation;
}
void Particle::setColor(vec3 newColor)
{
	color = newColor;
}
void Particle::setLifetime(int newLifetime)
{
	lifetime = newLifetime;
}
vert* Particle::particle2vert(Particle* p)
{
	vert verts[3] = {
						{ (p->getVertices())[0], (p->getNorm())[0], p->getCenter(), p->getColor(), p->getLifetime(), p->getSize() },
						{ (p->getVertices())[1], (p->getNorm())[1], p->getCenter(), p->getColor(), p->getLifetime(), p->getSize() },
						{ (p->getVertices())[2], (p->getNorm())[2], p->getCenter(), p->getColor(), p->getLifetime(), p->getSize() }					
					};
	return verts;
}