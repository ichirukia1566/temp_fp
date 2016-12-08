#include <iostream>
#include <random>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "gl_core_3_3.h"
#include <GL/freeglut.h>
#include "util.hpp"
#include "mesh.hpp"
#include "particle.h"
#define PI 3.14159265
using namespace std;
using namespace glm;

// Global state
GLint width, height;
unsigned int viewmode;	// View triangle or obj file
GLuint shader;			// Shader program
GLuint program_update;  // update shader
GLuint program_render;  // render shader
GLuint uniXform;		// Shader location of xform mtx
GLuint uniFlip;
GLuint uniSpeed;
GLuint uniT;
GLuint vao;				// Vertex array object
GLuint vbuf;			// Vertex buffer
GLsizei vcount;			// Number of vertices
Mesh* mesh;				// Mesh loaded from .obj file
// Camera state
vec3 camCoords;			// Spherical coordinates (theta, phi, radius) of the camera
bool camRot;			// Whether the camera is currently rotating
vec2 camOrigin;			// Original camera coordinates upon clicking
vec2 mouseOrigin;		// Original mouse coordinates upon clicking

int flip; // the variable that determines which triangle of the "new particle" is rendered
float speed;
int t; // time
vector<vert> verts;
int lastNewIndex;
GLuint uiTBO;
GLuint uiQuery;
GLuint uiParticleBuffer[2];
GLuint uiVAO[2];
int iCurReadBuffer = 0;
int iNumParticles;
GLuint unifTimePassed;
GLuint univGenPosition;
GLuint univGenVelocityMin;
GLuint univGenVelocityRange;
GLuint univGenColor;
GLuint univGenCurlVector;
GLuint unifGenLifeMin;
GLuint unifGenLifeRange;
GLuint unifGenSize;
GLuint uniiNumToGenerate;
GLuint uniP;
GLuint uniPerm;
GLuint uniGrad3;
GLuint testUBO;
float fElapsedTime = 0.8f;
float fNextGenerationTime = 0.02f;

vec3 vGenPosition = vec3(0.0f, 0.0f, 0.0f);
vec3 vGenVelocityMin = vec3(-0.5f, 0.0f, -0.5f), vGenVelocityRange = vec3(10.0f, 20.0f, 10.0f);
vec3 vGenCurlVector = vec3(0.0f, -0.5f, 0.0f);
vec3 vGenColor = vec3(0.0f, 0.5f, 1.0f);

float fGenLifeMin = 1.5f, fGenLifeRange = 100.0f;
float fGenSize = 0.05f;

int iNumToGenerate = 60;
mat4 matProjection, matView;
vec3 vQuad1, vQuad2;

// Constants
const int MENU_VIEWMODE = 0;		// Toggle view mode
const int MENU_EXIT = 1;			// Exit application
const int VIEWMODE_TRIANGLE = 0;	// View triangle
const int VIEWMODE_OBJ = 1;			// View obj-loaded mesh

// Initialization functions
void initState();
void initGLUT(int* argc, char** argv);
void initOpenGL();
void initTriangle();

// Callback functions
void display();
void reshape(GLint width, GLint height);
void keyRelease(unsigned char key, int x, int y);
void mouseBtn(int button, int state, int x, int y);
void mouseMove(int x, int y);
void idle();
void menu(int cmd);
void cleanup();

// noise functions
int grad3[12][3] = { { 1,1,0 },{ -1,1,0 },{ 1,-1,0 },{ -1,-1,0 },
				{ 1,0,1 },{ -1,0,1 },{ 1,0,-1 },{ -1,0,-1 },
				{ 0,1,1 },{ 0,-1,1 },{ 0,1,-1 },{ 0,-1,-1 } };
int grad4[32][4] = { { 0,1,1,1 },{ 0,1,1,-1 },{ 0,1,-1,1 },{ 0,1,-1,-1 },
				{ 0,-1,1,1 },{ 0,-1,1,-1 },{ 0,-1,-1,1 },{ 0,-1,-1,-1 },
				{ 1,0,1,1 },{ 1,0,1,-1 },{ 1,0,-1,1 },{ 1,0,-1,-1 },
				{ -1,0,1,1 },{ -1,0,1,-1 },{ -1,0,-1,1 },{ -1,0,-1,-1 },
				{ 1,1,0,1 },{ 1,1,0,-1 },{ 1,-1,0,1 },{ 1,-1,0,-1 },
				{ -1,1,0,1 },{ -1,1,0,-1 },{ -1,-1,0,1 },{ -1,-1,0,-1 },
				{ 1,1,1,0 },{ 1,1,-1,0 },{ 1,-1,1,0 },{ 1,-1,-1,0 },
				{ -1,1,1,0 },{ -1,1,-1,0 },{ -1,-1,1,0 },{ -1,-1,-1,0 } };
int grad3_1D[36] = { 1, 1, 0, -1, 1, 0, 1, -1, 0, -1, -1 , 0,
				1, 0, 1, -1, 0, 1, 1, 0, -1, -1, 0, -1,
				0, 1, 1, 0, -1, 1, 0, 1, -1, 0, -1, -1 };

int p[] = { 151,160,137,91,90,15,
131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180 };
int perm[512];
void initPerm();
int fastfloor(float x)
{
	return (x > 0) ? (int)x : (int)x - 1;
}
float dot(int g[], float x, float y, float z) 
{
	return g[0] * x + g[1] * y + g[2] * z;
}
float simplexNoise(float x, float y, float z)
{
	float n0, n1, n2, n3; // Noise contributions from the four corners
						   // Skew the input space to determine which simplex cell we're in
	float F3 = 1.0 / 3.0;
	float s = (x + y + z) * F3; // Very nice and simple skew factor for 3D
	int i = fastfloor(x + s);
	int j = fastfloor(y + s);
	int k = fastfloor(z + s);
	float G3 = 1.0 / 6.0; // Very nice and simple unskew factor, too
	float t = (i + j + k) * G3;
	float X0 = i - t; // Unskew the cell origin back to (x,y,z) space
	float Y0 = j - t;
	float Z0 = k - t;
	float x0 = x - X0; // The x,y,z distances from the cell origin
	float y0 = y - Y0;
	float z0 = z - Z0;
	// For the 3D case, the simplex shape is a slightly irregular tetrahedron.
	// Determine which simplex we are in.
	int i1, j1, k1; // Offsets for second corner of simplex in (i,j,k) coords
	int i2, j2, k2; // Offsets for third corner of simplex in (i,j,k) coords
	if (x0 >= y0) {
		if (y0 >= z0)
		{
			i1 = 1; j1 = 0; k1 = 0; i2 = 1; j2 = 1; k2 = 0;
		} // X Y Z order
		else if (x0 >= z0) { i1 = 1; j1 = 0; k1 = 0; i2 = 1; j2 = 0; k2 = 1; } // X Z Y order
		else { i1 = 0; j1 = 0; k1 = 1; i2 = 1; j2 = 0; k2 = 1; } // Z X Y order
	}
	else { // x0<y0
		if (y0<z0) { i1 = 0; j1 = 0; k1 = 1; i2 = 0; j2 = 1; k2 = 1; } // Z Y X order
		else if (x0<z0) { i1 = 0; j1 = 1; k1 = 0; i2 = 0; j2 = 1; k2 = 1; } // Y Z X order
		else { i1 = 0; j1 = 1; k1 = 0; i2 = 1; j2 = 1; k2 = 0; } // Y X Z order
	}
	// A step of (1,0,0) in (i,j,k) means a step of (1-c,-c,-c) in (x,y,z),
	// a step of (0,1,0) in (i,j,k) means a step of (-c,1-c,-c) in (x,y,z), and
	// a step of (0,0,1) in (i,j,k) means a step of (-c,-c,1-c) in (x,y,z), where
	// c = 1/6.
	float x1 = x0 - i1 + G3; // Offsets for second corner in (x,y,z) coords
	float y1 = y0 - j1 + G3;
	float z1 = z0 - k1 + G3;
	float x2 = x0 - i2 + 2.0*G3; // Offsets for third corner in (x,y,z) coords
	float y2 = y0 - j2 + 2.0*G3;
	float z2 = z0 - k2 + 2.0*G3;
	float x3 = x0 - 1.0 + 3.0*G3; // Offsets for last corner in (x,y,z) coords
	float y3 = y0 - 1.0 + 3.0*G3;
	float z3 = z0 - 1.0 + 3.0*G3;
	// Work out the hashed gradient indices of the four simplex corners
	int ii = i & 255;
	int jj = j & 255;
	int kk = k & 255;
	int gi0 = perm[ii + perm[jj + perm[kk]]] % 12;
	int gi1 = perm[ii + i1 + perm[jj + j1 + perm[kk + k1]]] % 12;
	int gi2 = perm[ii + i2 + perm[jj + j2 + perm[kk + k2]]] % 12;
	int gi3 = perm[ii + 1 + perm[jj + 1 + perm[kk + 1]]] % 12;
	// Calculate the contribution from the four corners
	float t0 = 0.6 - x0*x0 - y0*y0 - z0*z0;
	if (t0<0) n0 = 0.0;
	else {
		t0 *= t0;
		n0 = t0 * t0 * dot(grad3[gi0], x0, y0, z0);
	}
	float t1 = 0.6 - x1*x1 - y1*y1 - z1*z1;
	if (t1<0) n1 = 0.0;
	else {
		t1 *= t1;
		n1 = t1 * t1 * dot(grad3[gi1], x1, y1, z1);
	}
	float t2 = 0.6 - x2*x2 - y2*y2 - z2*z2;
	if (t2<0) n2 = 0.0;
	else {
		t2 *= t2;
		n2 = t2 * t2 * dot(grad3[gi2], x2, y2, z2);
	}
	float t3 = 0.6 - x3*x3 - y3*y3 - z3*z3;
	if (t3<0) n3 = 0.0;
	else {
		t3 *= t3;
		n3 = t3 * t3 * dot(grad3[gi3], x3, y3, z3);
	}
	// Add contributions from each corner to get the final noise value.
	// The result is scaled to stay just inside [-1,1]
	return 32.0*(n0 + n1 + n2 + n3);
}

vec3 simplexNoiseVec3(vec3 x)
{
	float s = simplexNoise(x.x, x.y, x.z);
	float s1 = simplexNoise(x.y - 19.1, x.z + 33.4, x.x + 47.2);
	float s2 = simplexNoise(x.z + 74.2, x.x - 124.5, x.y + 99.4);
	vec3 c = vec3(s, s1, s2);
	return c;
}

vec3 curlNoise(float x, float y, float z)
{
	/*const float e = 0.1f;
	vec3 dx = vec3(e, 0.0, 0.0);
	vec3 dy = vec3(0.0, e, 0.0);
	vec3 dz = vec3(0.0, 0.0, e);

	vec3 p_x0 = simplexNoiseVec3(vec3(x - dx.x, y - dx.y, z - dx.z));
	vec3 p_x1 = simplexNoiseVec3(vec3(x, y, z) + dx);
	vec3 p_y0 = simplexNoiseVec3(vec3(x, y, z) - dy);
	vec3 p_y1 = simplexNoiseVec3(vec3(x, y, z) + dy);
	vec3 p_z0 = simplexNoiseVec3(vec3(x, y, z) - dz);
	vec3 p_z1 = simplexNoiseVec3(vec3(x, y, z) + dz);

	float x_1 = p_y1.z - p_y0.z - p_z1.y + p_z0.y;
	float y_1 = p_z1.x - p_z0.x - p_x1.z + p_x0.z;
	float z_1 = p_x1.y - p_x0.y - p_y1.x + p_y0.x;

	const float divisor = 1.0 / (2.0 * e);
	return normalize(vec3(x_1, y_1, z_1) * divisor);*/

	float eps = 0.001f;
	float n1, n2, a, b;
	vec3 curl;
	n1 = simplexNoise(x, y + eps, z);
	n2 = simplexNoise(x, y - eps, z);
	a = (n1 - n2) / (2 * eps);

	n1 = simplexNoise(x, y, z + eps);
	n2 = simplexNoise(x, y, z - eps);
	b = (n1 - n2) / (2 * eps);

	curl.x = a - b;

	n1 = simplexNoise(x, y, z + eps);
	n2 = simplexNoise(x, y, z - eps);
	a = (n1 - n2) / (2 * eps);

	n1 = simplexNoise(x + eps, y, z);
	n2 = simplexNoise(x + eps, y, z);
	b = (n1 - n2) / (2 * eps);

	curl.y = a - b;
	n1 = simplexNoise(x + eps, y, z);
	n2 = simplexNoise(x - eps, y, z);
	a = (n1 - n2) / (2 * eps);

	n1 = simplexNoise(x, y + eps, z);
	n2 = simplexNoise(x, y - eps, z);
	b = (n1 - n2) / (2 * eps);

	curl.z = a - b;

	return curl;
}

int main(int argc, char** argv) {
	try {
		// Initialize
		initPerm();
		initState();
		initGLUT(&argc, argv);
		initOpenGL();
		initTriangle();

	} catch (const exception& e) {
		// Handle any errors
		cerr << "Fatal error: " << e.what() << endl;
		cleanup();
		return -1;
	}

	// Execute main loop
	glutMainLoop();

	return 0;
}
void initPerm()
{
	for (int i = 0; i<512; i++) 
		perm[i] = p[i & 255];
}
void initState() {
	// Initialize global state
	width = 0;
	height = 0;
	viewmode = VIEWMODE_TRIANGLE;
	shader = 0;
	uniXform = 0;
	uniFlip = 1;
	uniSpeed = 2;
	uniT = 3;
	vao = 0;
	vbuf = 0;
	vcount = 0;
	mesh = NULL;
	flip = 0;
	speed = 0.0f;
	lastNewIndex = 0;
	camCoords = vec3(0.0, 0.0, 20.0);
	camRot = false;
}

void initGLUT(int* argc, char** argv) {
	// Set window and context settings
	width = 800; height = 600;
	glutInit(argc, argv);
	glutInitWindowSize(width, height);
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	// Create the window
	glutCreateWindow("FreeGlut Window");

	// Create a menu
	glutCreateMenu(menu);
	glutAddMenuEntry("Toggle view mode", MENU_VIEWMODE);
	glutAddMenuEntry("Exit", MENU_EXIT);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	// GLUT callbacks
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardUpFunc(keyRelease);
	glutMouseFunc(mouseBtn);
	glutMotionFunc(mouseMove);
	glutIdleFunc(idle);
	glutCloseFunc(cleanup);
}

void initOpenGL() {
	const char* sVaryings[7] =
	{
		"vCenterOut",
		"vColorOut",
		"vVelocityOut",
		"vCurlOut",
		"fSizeOut",
		"fLifetimeOut",
		"iTypeOut"
	};

	//// Set clear color and depth
	//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	//glClearDepth(1.0f);
	//// Enable depth testing
	//glEnable(GL_DEPTH_TEST);

	// Compile and link shader program
	vector<GLuint> shaders;
	shaders.push_back(compileShader(GL_VERTEX_SHADER, "sh_v.glsl"));
	shaders.push_back(compileShader(GL_GEOMETRY_SHADER, "sh_g.glsl"));
	//shaders.push_back(compileShader(GL_FRAGMENT_SHADER, "sh_f.glsl"));
	program_update = glCreateProgram();
	// Attach the shaders and link the program
	for (auto it = shaders.begin(); it != shaders.end(); ++it)
		glAttachShader(program_update, *it);
	//for (int i = 0; i < 7; i++) 
		glTransformFeedbackVaryings(program_update, 7, sVaryings, GL_INTERLEAVED_ATTRIBS);
	glLinkProgram(program_update);
	// Release shader sources
	for (auto s = shaders.begin(); s != shaders.end(); ++s)
		glDeleteShader(*s);
	shaders.clear();

	shaders.push_back(compileShader(GL_VERTEX_SHADER, "render_v.glsl"));
	shaders.push_back(compileShader(GL_GEOMETRY_SHADER, "render_g.glsl"));
	shaders.push_back(compileShader(GL_FRAGMENT_SHADER, "render_f.glsl"));
	program_render = linkProgram(shaders);
	for (auto s = shaders.begin(); s != shaders.end(); ++s)
		glDeleteShader(*s);
	shaders.clear();
	
	//glGenTransformFeedbacks(1, &uiTransformFeedbackBuffer);
	glGenBuffers(1, &uiTBO);
	glBindBuffer(GL_ARRAY_BUFFER, uiTBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vert) * 100000, nullptr, GL_STATIC_READ);

	glGenQueries(1, &uiQuery);

	glGenBuffers(2, uiParticleBuffer);
	glGenVertexArrays(2, uiVAO);

	Particle partInitialization(1000000.0f, vec3(-10.0f, 17.5f, 0.0f), 0.1f, vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.5f, 1.0f), 0);
	//Particle partInitialization1(1000000.0f, vec3(-10.0f, 17.5f, 0.0f), 0.1f, vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.5f, 1.0f), 0);
	partInitialization.setType(0);
	verts.push_back(partInitialization.particle2vert(vec3(0.0f)));
	//verts.push_back(partInitialization1.particle2vert(vec3(0.0f)));

	for (int i = 0; i < 2; i++)
	{
		glBindVertexArray(uiVAO[i]);
		glBindBuffer(GL_ARRAY_BUFFER, uiParticleBuffer[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vert)* 100000, NULL, GL_DYNAMIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vert), verts.data());

		for (int i = 0; i < 7; i++)
			glEnableVertexAttribArray(i);
		
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vert), 0); // vec3 center
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vert), (GLvoid*)sizeof(vec3)); // vec3 color
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vert), (GLvoid*)(2 * sizeof(vec3))); // vec3 velocity
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(vert), (GLvoid*)(3 * sizeof(vec3))); // vec3 curl
		glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(vert), (GLvoid*)(4 * sizeof(vec3))); // float size
		glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(vert), (GLvoid*)(4 * sizeof(vec3) + sizeof(float))); // float lifetime
		glVertexAttribIPointer(6, 1, GL_INT, sizeof(vert), (GLvoid*)(4 * sizeof(vec3) + 2 * sizeof(float))); // int type
	}
	iCurReadBuffer = 0;
	iNumParticles = 1;

	//bInitialized = true;

	//return true;

}

float grandf(float fMin, float fAdd)
{
	float fRandom = float(rand() % (RAND_MAX + 1)) / float(RAND_MAX);
	return fMin + fAdd * fRandom;
}

void updateParticles(float fTimePassed)
{
	glUseProgram(program_update);

	unifTimePassed = glGetUniformLocation(program_update, "fTimePassed");
	glUniform1fv(unifTimePassed, 1, &fTimePassed);
	univGenPosition = glGetUniformLocation(program_update, "vGenPosition");
	glUniform3fv(univGenPosition, 1, (GLfloat*)&vGenPosition);
	univGenVelocityMin = glGetUniformLocation(program_update, "vGenVelocityMin");
	glUniform3fv(univGenVelocityMin, 1, (GLfloat*)&vGenVelocityMin);
	univGenVelocityRange = glGetUniformLocation(program_update, "vGenVelocityRange");
	glUniform3fv(univGenVelocityRange, 1, (GLfloat*)&vGenVelocityRange);
	univGenColor = glGetUniformLocation(program_update, "vGenColor");
	glUniform3fv(univGenColor, 1, (GLfloat*)&vGenColor);
	univGenCurlVector = glGetUniformLocation(program_update, "vGenCurlVector");
	glUniform3fv(univGenCurlVector, 1, (GLfloat*)&vGenCurlVector);
	unifGenLifeMin = glGetUniformLocation(program_update, "fGenLifeMin");
	glUniform1fv(unifGenLifeMin, 1, &fGenLifeMin);
	unifGenLifeRange = glGetUniformLocation(program_update, "fGenLifeRange");
	glUniform1fv(unifGenLifeRange, 1, &fGenLifeRange);
	unifGenSize = glGetUniformLocation(program_update, "fGenSize");
	glUniform1fv(unifGenSize, 1, &fGenSize);
	uniiNumToGenerate = glGetUniformLocation(program_update, "iNumToGenerate");
	glUniform1i(uniiNumToGenerate, 0);
	uniP = glGetUniformLocation(program_update, "p");
	glUniform1iv(uniP, 256, p);
	//uniPerm = glGetUniformLocation(program_update, "perm");
	//glUniform1iv(uniPerm, 512, p);
	uniGrad3 = glGetUniformLocation(program_update, "grad3");
	glUniform1iv(uniGrad3, 36, grad3_1D);

	glGenBuffers(1, &testUBO);

	// Allocate storage for the UBO
	glBindBuffer(GL_UNIFORM_BUFFER, testUBO);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(GLint) * 512, perm, GL_DYNAMIC_DRAW);
	GLuint myArrayBlockIdx = glGetUniformBlockIndex(program_update, "perm");

	glUniformBlockBinding(program_update, myArrayBlockIdx, 0);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, testUBO);
	
	fElapsedTime += fTimePassed;

	if(fElapsedTime > fNextGenerationTime)
	{
		//spUpdateParticles.SetUniform("iNumToGenerate", iNumToGenerate);
		uniiNumToGenerate = glGetUniformLocation(program_update, "iNumToGenerate");
		glUniform1i(uniiNumToGenerate, iNumToGenerate);
		fElapsedTime -= fNextGenerationTime;

		vec3 vRandomSeed = glm::vec3(grandf(-10.0f, 20.0f), grandf(-10.0f, 20.0f), grandf(-10.0f, 20.0f));
		glUniform3fv(glGetUniformLocation(program_update, "vRandomSeed"), 1, (GLfloat*)&vRandomSeed);
	}

	glEnable(GL_RASTERIZER_DISCARD);
	//glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, uiTransformFeedbackBuffer);

	glBindVertexArray(uiVAO[iCurReadBuffer]);
	glEnableVertexAttribArray(2); // Re-enable velocity

	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, uiParticleBuffer[1 - iCurReadBuffer]);

	glBeginQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, uiQuery);
	glBeginTransformFeedback(GL_POINTS);
	
	glDrawArrays(GL_POINTS, 0, iNumParticles);

	glEndTransformFeedback();

	glEndQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN);
	glGetQueryObjectiv(uiQuery, GL_QUERY_RESULT, &iNumParticles);
	//cout << iNumParticles << endl;
	//vert feedback[sizeof(vert) * 100000];
	//glGetBufferSubData(GL_TRANSFORM_FEEDBACK_BUFFER, 0, sizeof(feedback), feedback);
	//cout << "lol\n";
	//printf("%f\n", feedback[0].center);
	//void * data = glMapBufferRange(GL_ARRAY_BUFFER, 0, sizeof(vert) * 1000000, GL_MAP_READ_BIT);
	//cout << data;
	iCurReadBuffer = 1 - iCurReadBuffer;

	//glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
}

void renderParticles()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glDepthMask(0);

	glDisable(GL_RASTERIZER_DISCARD);
	glUseProgram(program_render);

	glUniformMatrix4fv(glGetUniformLocation(program_render, "matrices.mProj"), 1, FALSE, (GLfloat*)&matProjection);
	glUniformMatrix4fv(glGetUniformLocation(program_render, "matrices.mView"), 1, FALSE, (GLfloat*)&matView);
	glUniform3fv(glGetUniformLocation(program_render, "vQuad1"), 1, (GLfloat*)&vQuad1);
	glUniform3fv(glGetUniformLocation(program_render, "vQuad2"), 1, (GLfloat*)&vQuad2);
	glUniform1i(glGetUniformLocation(program_render, "gSampler"), 0);
	glUniform1i(glGetUniformLocation(program_render, "flip"), flip);

	glBindVertexArray(uiVAO[iCurReadBuffer]);
	//void * data = glMapBufferRange(GL_ARRAY_BUFFER, 0, sizeof(vert) * 1000000, GL_MAP_READ_BIT);
	//cout << data;
	glDisableVertexAttribArray(2); // Disable velocity, because we don't need it for rendering

	glDrawArrays(GL_POINTS, 0, iNumParticles);

	glDepthMask(1);
	glDisable(GL_BLEND);
}

void initTriangle() {
	
	//Particle* particle1 = new Particle(10000, vec3(0.0f, 0.289f, 0.0f), 1.0f, 0.0f, vec3(1.0f, 0.0f, 0.0f));
	//Particle* particle2 = new Particle(20000, vec3(0.0f, 0.0f, 0.0f), 3.0f, 0.0f, vec3(0.0f, 1.0f, 0.0f));
	// Create a colored triangle
	/*
	struct vert {
		vec3 center;
		vec3 color;
		vec3 velocity;
		vec3 curl;
		float size;
		float lifetime;
		int type;
	};*/
	verts = { //(particle1->particle2vert())[0], (particle1->particle2vert())[1], (particle1->particle2vert())[2],
						  //(particle2->particle2vert())[0], (particle2->particle2vert())[1], (particle2->particle2vert())[2]
							};
	//std::random_device rd;
	//

	//for (int i = 0; i < 1000000; i++)
	//{
	//	std::mt19937 eng(rd());
	//	std::uniform_real_distribution<> distr(0, 1);
	//	float rand_lifetime = distr(eng);
	//	float rand_center1 = distr(eng);
	//	float rand_center2 = distr(eng);
	//	float rand_center3 = distr(eng);
	//	float rand_size = distr(eng);
	//	//float rand_rotation = PI * distr(eng);
	//	float rand_color1 = distr(eng);
	//	float rand_color2 = distr(eng);
	//	float rand_color3 = distr(eng);
	//	float rand_velocity1 = distr(eng);
	//	float rand_velocity2 = distr(eng);
	//	float rand_velocity3 = distr(eng);

	//	//Particle(float l, vec3 c, float s, vec3 col, vec3 v, int t);
	//	Particle* rand_particle = new Particle(10000.0f * rand_lifetime, vec3(1.0f * rand_center1, 1.0f * rand_center2, 1.0f * rand_center3), 
	//											1.0f * rand_size, vec3(rand_color1, rand_color2, rand_color3), vec3(rand_velocity1, rand_velocity2, rand_velocity3), 1);
	//	vec3 curl = curlNoise(simplexNoise(rand_center1, rand_center2, rand_center3), simplexNoise(rand_center1, rand_center2, rand_center3), simplexNoise(rand_center1, rand_center2, rand_center3));
	//	//cout << curl.x << ", " << curl.y << ", " << curl.z << endl;
	//	verts.push_back(rand_particle->particle2vert(curl));
	//}
	//vcount = verts.size();

	// Create vertex array object
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create vertex buffer
	glGenBuffers(1, &vbuf);
	glBindBuffer(GL_ARRAY_BUFFER, vbuf);
	glBufferData(GL_ARRAY_BUFFER, vcount * sizeof(vert), verts.data(), GL_STATIC_DRAW);
	// Specify vertex attributes
	/*
	struct vert {
		vec3 center;
		vec3 color;
		vec3 velocity;
		vec3 curl;
		float size;
		float lifetime;
		int type;
	};*/
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vert), 0); // vec3 center
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vert), (GLvoid*)sizeof(vec3)); // vec3 color
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vert), (GLvoid*)(2 * sizeof(vec3))); // vec3 velocity
	glEnableVertexAttribArray(3); 
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(vert), (GLvoid*)(3 * sizeof(vec3))); // vec3 curl
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(vert), (GLvoid*)(4 * sizeof(vec3))); // float size
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(vert), (GLvoid*)(4 * sizeof(vec3) + sizeof(float))); // float lifetime
	glEnableVertexAttribArray(6);
	glVertexAttribIPointer(6, 1, GL_INT, sizeof(vert), (GLvoid*)(4 * sizeof(vec3) + 2 * sizeof(float))); // int type
	

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void display() {
	float aspect = (float)width / (float)height;
	mat4 proj = perspective(45.0f, aspect, 0.1f, 100.0f);
	//mat4 view = translate(mat4(), vec3(0.0, 0.0, -camCoords.z));
	matProjection = proj;
	vec3 vEye = camCoords;
	vec3 vView = vec3(0.0f, 0.0f,-1.0f);
	vec3 vUp = vec3(0.0f, 1.0f, 0.0f);
	matView = lookAt(vEye, vView, vUp);
	vView = vView - vEye;
	vView = normalize(vView);
	vQuad1 = cross(vView, vUp);
	vQuad1 = normalize(vQuad1);
	vQuad2 = cross(vView, vQuad1);
	vQuad2 = normalize(vQuad2);
	updateParticles(0.05f);
	renderParticles();
	glFlush();


	//try {
	//	// Clear the back buffer
	//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//	// Get ready to draw
	//	glUseProgram(shader);

	//	mat4 xform;
	//	float aspect = (float)width / (float)height;
	//	// Create perspective projection matrix
	//	mat4 proj = perspective(45.0f, aspect, 0.1f, 100.0f);
	//	// Create view transformation matrix
	//	mat4 view = translate(mat4(), vec3(0.0, 0.0, -camCoords.z));
	//	mat4 rot = rotate(mat4(), radians(camCoords.y), vec3(1.0, 0.0, 0.0));
	//	rot = rotate(rot, radians(camCoords.x), vec3(0.0, 1.0, 0.0));
	//	xform = proj * view * rot;

	//	

	//	switch (viewmode) {
	//	case 0:
	//		glBindVertexArray(vao);
	//		// Send transformation matrix to shader
	//		glUniformMatrix4fv(uniXform, 1, GL_FALSE, value_ptr(xform));
	//		glUniform1i(uniFlip, flip);
	//		glUniform1f(uniSpeed, speed);
	//		glUniform1i(uniT, t);
	//		// Draw the triangle
	//		glDrawArrays(GL_TRIANGLES, 0, vcount);
	//		glBindVertexArray(0);
	//		break;

	//	case 1: {
	//		// Load model on demand
	//		if (!mesh) mesh = new Mesh("models/cow.obj");

	//		// Scale and center mesh using bounding box
	//		pair<vec3, vec3> meshBB = mesh->boundingBox();
	//		mat4 fixBB = scale(mat4(), vec3(1.0f / length(meshBB.second - meshBB.first)));
	//		fixBB = glm::translate(fixBB, -(meshBB.first + meshBB.second) / 2.0f);
	//		// Concatenate all transformations and upload to shader
	//		xform = xform * fixBB;
	//		glUniformMatrix4fv(uniXform, 1, GL_FALSE, value_ptr(xform));
	//		glUniform1i(uniFlip, flip);
	//		glUniform1f(uniSpeed, speed);
	//		glUniform1i(uniT, t);
	//		// Draw the mesh
	//		mesh->draw();
	//		break; }
	//	}

		// Revert context state
		glUseProgram(0);

		// Display the back buffer
		glutSwapBuffers();

	/*} catch (const exception& e) {
		cerr << "Fatal error: " << e.what() << endl;
		glutLeaveMainLoop();
	}*/
}

void reshape(GLint width, GLint height) {
	::width = width;
	::height = height;
	glViewport(0, 0, width, height);
}

void keyRelease(unsigned char key, int x, int y) {
	switch (key) {
	case 27:	// Escape key
		menu(MENU_EXIT);
		break;
	}
}

void mouseBtn(int button, int state, int x, int y) {
	if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON) {
		// Activate rotation mode
		camRot = true;
		camOrigin = vec2(camCoords);
		mouseOrigin = vec2(x, y);
	}
	if (state == GLUT_UP && button == GLUT_LEFT_BUTTON) {
		// Deactivate rotation
		camRot = false;
	}
	if (button == 3) {
		camCoords.z = clamp(camCoords.z - 0.1f, 0.1f, 10.0f);
		glutPostRedisplay();
	}
	if (button == 4) {
		camCoords.z = clamp(camCoords.z + 0.1f, 0.1f, 10.0f);
		glutPostRedisplay();
	}
}

void mouseMove(int x, int y) {
	if (camRot) {
		// Convert mouse delta into degrees, add to rotation
		float rotScale = min(width / 450.0f, height / 270.0f);
		vec2 mouseDelta = vec2(x, y) - mouseOrigin;
		vec2 newAngle = camOrigin + mouseDelta / rotScale;
		newAngle.y = clamp(newAngle.y, -90.0f, 90.0f);
		while (newAngle.x > 180.0f) newAngle.x -= 360.0f;
		while (newAngle.y < -180.0f) newAngle.y += 360.0f;
		if (length(newAngle - vec2(camCoords)) > FLT_EPSILON) {
			camCoords.x = newAngle.x;
			camCoords.y = newAngle.y;
			glutPostRedisplay();
		}
	}
}

void idle() {
	if (flip == 0)
	{
		flip = 1;
	}
	else
	{
		flip = 0;
	}
	//vector<vert> temp;

	//int index;
	//for (int i = lastNewIndex; i < verts.size(); i++)
	//{
	//	verts[i].lifetime--;
	//	verts[i].curl = curlNoise(simplexNoise(verts[i].center.x + ((verts[i].color).x + 1.0f * (verts[i].curl).x) * -0.01f * t, 
	//										verts[i].center.y + ((verts[i].color).y + 1.0f * (verts[i].curl).y) * -0.01f * t, 
	//										verts[i].center.z + ((verts[i].color).z + 1.0f * (verts[i].curl).z) * -0.01f * t), 
	//								simplexNoise(verts[i].center.x + ((verts[i].color).x + 1.0f * (verts[i].curl).x) * -0.01f * t,
	//									verts[i].center.y + ((verts[i].color).y + 1.0f * (verts[i].curl).y) * -0.01f * t,
	//									verts[i].center.z + ((verts[i].color).z + 1.0f * (verts[i].curl).z) * -0.01f * t),
	//								simplexNoise(verts[i].center.x + ((verts[i].color).x + 1.0f * (verts[i].curl).x) * -0.01f * t,
	//									verts[i].center.y + ((verts[i].color).y + 1.0f * (verts[i].curl).y) * -0.01f * t,
	//									verts[i].center.z + ((verts[i].color).z + 1.0f * (verts[i].curl).z) * -0.01f * t));
	//	if (verts[i].lifetime < 0)
	//	{

	//		//index = i;
	//		std::random_device rd;
	//		std::mt19937 eng(rd());
	//		std::uniform_real_distribution<> distr(0, 1);
	//		float rand_lifetime = distr(eng);
	//		float rand_center1 = distr(eng);
	//		float rand_center2 = distr(eng);
	//		float rand_center3 = distr(eng);
	//		float rand_size = distr(eng);
	//		float rand_rotation = PI * distr(eng);
	//		float rand_color1 = distr(eng);
	//		float rand_color2 = distr(eng);
	//		float rand_color3 = distr(eng);
	//		Particle* rand_particle = new Particle(10000 * rand_lifetime, vec3(rand_center1, rand_center2, rand_center3), 1.0f * rand_size, rand_rotation, vec3(rand_color1, rand_color2, rand_color3));
	//		vec3 curl = curlNoise(simplexNoise(rand_center1, rand_center2, rand_center3), simplexNoise(rand_center1, rand_center2, rand_center3), simplexNoise(rand_center1, rand_center2, rand_center3));
	//		verts[i] = (rand_particle->particle2vert(curl))[0];
	//		verts[i + 1] = (rand_particle->particle2vert(curl))[1];
	//		verts[i + 2] = (rand_particle->particle2vert(curl))[2];
	//		//temp.push_back((rand_particle->particle2vert())[0]);
	//		//temp.push_back((rand_particle->particle2vert())[1]);
	//		//temp.push_back((rand_particle->particle2vert())[2]);
	//		lastNewIndex = i + 3;
	//		if (lastNewIndex >= verts.size())
	//		{
	//			lastNewIndex = 0;
	//		}
	//		break;
	//	}

	//}
	//glBindVertexArray(vao);
	//glBindBuffer(GL_ARRAY_BUFFER, vbuf);
	//glBufferData(GL_ARRAY_BUFFER, vcount * sizeof(vert), NULL, GL_STATIC_DRAW);
	////glBufferSubData(GL_ARRAY_BUFFER, 0, vcount * sizeof(vert), verts.data());


	//glBindVertexArray(0);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	vGenPosition += vec3(cos(t), sin(t), 0.0f);
	speed += 0.0001f;
	t += 1;
	
	glutPostRedisplay();
}

void menu(int cmd) {
	switch (cmd) {
	case MENU_VIEWMODE:
		viewmode = (viewmode + 1) % 2;
		glutPostRedisplay();	// Tell GLUT to redraw the screen
		break;

	case MENU_EXIT:
		glutLeaveMainLoop();
		break;
	}
}

void cleanup() {
	// Release all resources
	if (shader) { glDeleteProgram(shader); shader = 0; }
	uniXform = 0;
	if (vao) { glDeleteVertexArrays(1, &vao); vao = 0; }
	if (vbuf) { glDeleteBuffers(1, &vbuf); vbuf = 0; }
	vcount = 0;
	if (mesh) { delete mesh; mesh = NULL; }
}