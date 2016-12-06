#version 330

smooth in vec3 fragNorm;	// Interpolated model-space normal

out vec3 outCol;	// Final pixel color

void main() {
	// Visualize normals as colors
	outCol = normalize(fragNorm);
	//outCol = vec3(0.5f, 0.0f, 0.5f);
}