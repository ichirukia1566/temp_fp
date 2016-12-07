#version 330 

uniform sampler2D gSampler; 

smooth in vec2 vTexCoord; 
flat in vec4 vColorPart; 

out vec4 FragColor; 

void main() 
{ 
 // vec4 vTexColor = texture2D(gSampler, vTexCoord); 
  FragColor = normalize(vColorPart); 
}