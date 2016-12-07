#version 330

smooth in vec3 fragPos;
smooth in vec3 fragNorm;	// Interpolated model-space normal
smooth in vec3 fragCol;

out vec3 outCol;	// Final pixel color

void main() {
	// Visualize normals as colors
	//outCol = normalize(normalize(fragNorm) * 0.5f + fragCol * 0.5f);
	outCol = vec3(198.0f / 256.0f, 242.0f / 256.0f, 230.0f / 256.0f);
	//outCol = fragCol;
}