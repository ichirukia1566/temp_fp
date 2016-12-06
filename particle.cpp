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
	mat4 temp_translation = mat4(1.0f, 0.0f, 0.0f, 0.0f,
								0.0f, 1.0f, 0.0f, 0.0f,
								0.0f, 0.0f, 1.0f, 0.0f,
								-c.x, -c.y, -c.z, 1.0f);
	mat4 temp_translation_inv = mat4(1.0f, 0.0f, 0.0f, 0.0f,
									0.0f, 1.0f, 0.0f, 0.0f,
									0.0f, 0.0f, 1.0f, 0.0f,
									c.x, c.y, c.z, 1.0f);
	mat4 temp_rotation = mat4(cos(r), 0.0f, -sin(r), 0.0f,
							  0.0f, 1.0f, 0.0f, 0.0f,
							  sin(r), 0.0f, cos(r), 0.0f,
							  0.0f, 0.0f, 0.0f, 1.0f);
	vertices[0] = c + vec3(0.0f, sqrt(3) * s / 3.0f, 0.0f);
	vertices[1] = vec3(temp_translation_inv * temp_rotation * temp_translation * vec4(c + vec3(-s * 0.5f, -sqrt(3) * s / 6.0f, 0.0f), 1.0f));
	vertices[2] = vec3(temp_translation_inv * temp_rotation * temp_translation * vec4(c + vec3(s * 0.5f, -sqrt(3) * s / 6.0f, 0.0f), 1.0f));
	norm[0] = normalize(cross(vertices[1] - vertices[0], vertices[2] - vertices[0]));
	norm[1] = normalize(cross(vertices[2] - vertices[1], vertices[0] - vertices[1]));
	norm[2] = normalize(cross(vertices[0] - vertices[2], vertices[1] - vertices[2]));
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
	for (int i = 0; i < 3; i++)
	{
		vertices[i] += (newCenter - center);
	}
	norm[0] = normalize(cross(vertices[1] - vertices[0], vertices[2] - vertices[0]));
	norm[1] = normalize(cross(vertices[2] - vertices[1], vertices[0] - vertices[1]));
	norm[2] = normalize(cross(vertices[0] - vertices[2], vertices[1] - vertices[2]));
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
	}
	norm[0] = normalize(cross(vertices[1] - vertices[0], vertices[2] - vertices[0]));
	norm[1] = normalize(cross(vertices[2] - vertices[1], vertices[0] - vertices[1]));
	norm[2] = normalize(cross(vertices[0] - vertices[2], vertices[1] - vertices[2]));
	size = newSize;
}
void Particle::setRotation(float newRotation)
{
	float diff = newRotation - rotation;
	mat4 temp_translation = mat4(1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		-center.x, -center.y, -center.z, 1.0f);
	mat4 temp_translation_inv = mat4(1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		center.x, center.y, center.z, 1.0f);
	mat4 temp_rotation = mat4(cos(diff), 0.0f, -sin(diff), 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		sin(diff), 0.0f, cos(diff), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
	for (int i = 0; i < 3; i++)
	{
		vertices[i] = vec3(temp_translation_inv * temp_rotation * temp_translation * vec4(vertices[i], 1.0f));
	}
	norm[0] = normalize(cross(vertices[1] - vertices[0], vertices[2] - vertices[0]));
	norm[1] = normalize(cross(vertices[2] - vertices[1], vertices[0] - vertices[1]));
	norm[2] = normalize(cross(vertices[0] - vertices[2], vertices[1] - vertices[2]));
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
vert* Particle::particle2vert(vec3 curl)
{
	vert verts[3] = {
						{ (this->getVertices())[0], (this->getNorm())[0], this->getCenter(), this->getColor(), this->getLifetime(), this->getSize(), this->getLifetime(), curl },
						{ (this->getVertices())[1], (this->getNorm())[1], this->getCenter(), this->getColor(), this->getLifetime(), this->getSize(), this->getLifetime(), curl },
						{ (this->getVertices())[2], (this->getNorm())[2], this->getCenter(), this->getColor(), this->getLifetime(), this->getSize(), this->getLifetime(), curl }					
					};
	return verts;
}