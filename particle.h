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
		vec3 pos;	// Vertex position
		vec3 norm;	// Vertex normal
		vec3 center;
		vec3 color;
		int lifetime;
		float size;
		int maxLifetime;
	};

class Particle {
public:
	Particle(int l, vec3 c, float s, float r, vec3 col);
	vec3* getVertices();
	vec3 getColor();
	vec3* getNorm();
	int getLifetime();
	float getSize();
	float getRotation();
	vec3 getCenter();
	void setCenter(vec3 newCenter);
	void setSize(float newSize);
	void setRotation(float newRotation);
	void setColor(vec3 newColor);
	void setLifetime(int newLifetime);
	vert* particle2vert();
protected:
private:
	int lifetime;
	vec3 center; // center of triangle
	float size; // length of edge
	float rotation; // rotation around y axis, clockwise
	vec3 vertices[3]; 
	vec3 color; 
	vec3 norm[3];
};