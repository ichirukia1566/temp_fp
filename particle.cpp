#include "particle.h"

using namespace std;
using namespace glm;

Particle::Particle(float l, vec3 c, float s, vec3 col, vec3 v, int t)
{
	lifetime = l;
	center = c;
	size = s;
	color = col;
	velocity = v;
	type = t;
}

vec3 Particle::getColor()
{
	return color;
}

float Particle::getLifetime()
{
	return lifetime;
}

float Particle::getSize()
{
	return size;
}

vec3 Particle::getCenter()
{
	return center;
}

vec3 Particle::getVelocity()
{
	return velocity;
}

int Particle::getType()
{
	return type;
}

void Particle::setCenter(vec3 newCenter)
{
	center = newCenter;
}
void Particle::setSize(float newSize)
{
	size = newSize;
}

void Particle::setColor(vec3 newColor)
{
	color = newColor;
}
void Particle::setLifetime(float newLifetime)
{
	lifetime = newLifetime;
}

void Particle::setVelocity(vec3 v)
{
	velocity = v;
}

void Particle::setType(int t)
{
	type = t;
}

/*
struct vert {
	vec3 center;
	vec3 color;
	vec3 velocity;
	vec3 curl;
	float size;
	float lifetime;
	int type;
};
*/

vert Particle::particle2vert(vec3 curl)
{
	vert vert = { this->getCenter(), this->getColor(), this->getVelocity(), curl, this->getSize(), this->getType() };
	return vert;
}