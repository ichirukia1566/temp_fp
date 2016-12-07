#version 330

layout(points) in;
layout(points, max_vertices = 60) out;

// input from vertex shader
in vec3 vCenterPass[];
in vec3 vColorPass[];
in vec3 vVelocityPass[];
in vec3 vCurlPass[];
in float fSizePass[];
in float fLifetimePass[];
flat in int iTypePass[];

smooth out vec3 vCenterOut;
smooth out vec3 vColorOut;
smooth out vec3 vVelocityOut;
smooth out vec3 vCurlOut;
out float fSizeOut;
out float fLifetimeOut;
flat out int iTypeOut;


uniform vec3 vGenPosition;
uniform vec3 vGenCurlVector;
uniform vec3 vGenVelocityMin;
uniform vec3 vGenVelocityRange;

uniform vec3 vGenColor; 
uniform float fGenSize; 

uniform float fGenLifeMin, fGenLifeRange; // Life of new particle - from min to (min+range)
uniform float fTimePassed; // Time passed since last frame

uniform vec3 vRandomSeed; // Seed number for our random number function
vec3 vLocalSeed; 

uniform int iNumToGenerate; // How many particles will be generated next time, if greater than zero, particles are generated

// This function returns random number from zero to one
float randZeroOne() 
{ 
    uint n = floatBitsToUint(vLocalSeed.y * 214013.0 + vLocalSeed.x * 2531011.0 + vLocalSeed.z * 141251.0); 
    n = n * (n * n * 15731u + 789221u); 
    n = (n >> 9u) | 0x3F800000u; 
  
    float fRes =  2.0 - uintBitsToFloat(n); 
    vLocalSeed = vec3(vLocalSeed.x + 147158.0 * fRes, vLocalSeed.y*fRes  + 415161.0 * fRes, vLocalSeed.z + 324154.0*fRes); 
    return fRes; 
} 

void main()
{
	vLocalSeed = vRandomSeed;

	vCenterOut = vCenterPass[0];
	vVelocityOut = vVelocityPass[0]; 
	if(iTypePass[0] != 0)vCenterOut += vVelocityOut*fTimePassed; 
	if(iTypePass[0] != 0)vVelocityOut += vGenCurlVector*fTimePassed; 

	vColorOut = vColorPass[0]; 
	fLifetimeOut = fLifetimePass[0]-fTimePassed; 
	fSizeOut = fSizePass[0]; 
	iTypeOut = iTypePass[0]; 

	if(iTypeOut == 0) 
	{ 
		EmitVertex(); 
		EndPrimitive(); 
     
		for(int i = 0; i < iNumToGenerate; i++) 
		{ 
			vCenterOut = vGenPosition; 
			vVelocityOut = vGenVelocityMin+vec3(vGenVelocityRange.x*randZeroOne(), vGenVelocityRange.y*randZeroOne(), vGenVelocityRange.z*randZeroOne()); 
			vColorOut = vGenColor;
			vCurlOut = vGenCurlVector; 
			fLifetimeOut = fGenLifeMin+fGenLifeRange*randZeroOne(); 
			fSizeOut = fGenSize; 
			iTypeOut = 1; 
			EmitVertex(); 
			EndPrimitive(); 
		} 
	} 
	else if(fLifetimeOut > 0.0) 
	{ 
		EmitVertex(); 
		EndPrimitive();  
	} 
}