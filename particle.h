#pragma once
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "gl_core_3_3.h"
#include <GL/freeglut.h>
#include "util.hpp"
#include "mesh.hpp"

using namespace std;
using namespace glm;

struct vert {
		vec3 center;
		vec3 color;
		vec3 velocity;
		vec3 curl;
		float size;
		float lifetime;
		int type;
	};

class Particle {
public:
	Particle(float l, vec3 c, float s, vec3 col, vec3 v, int t);
	//vec3* getVertices();
	vec3 getColor();
	//vec3* getNorm();
	float getLifetime();
	float getSize();
	//float getRotation();
	vec3 getCenter();
	vec3 getVelocity();
	int getType();
	void setCenter(vec3 newCenter);
	void setSize(float newSize);
	//void setRotation(float newRotation);
	void setColor(vec3 newColor);
	void setLifetime(float newLifetime);
	void setVelocity(vec3 v);
	void setType(int t);
	vert particle2vert(vec3 curl);
protected:
private:
	float lifetime;
	vec3 center; // center of triangle
	float size; // length of edge
	//float rotation; // rotation around y axis, clockwise
	//vec3 vertices[3]; 
	vec3 color; 
	//vec3 norm[3];
	vec3 velocity;
	int type;
};