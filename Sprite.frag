#version 130
// The fragment shader operates on each pixel in a given polygon.
// out for output and in for input

in vec4 fragmentColor;
in vec2 fragmentPosition;
in vec2 fragmentUV;

out vec4 color; 						// output color vector
uniform sampler2D ns2TextureSampler;	// shader texture 

void main() 
{
	vec4 textureColor = texture(ns2TextureSampler, fragmentUV);
	color = fragmentColor * textureColor;
}