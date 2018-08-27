#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Norm;

// texture sampler
uniform sampler2D atlas;
uniform float transparency;

void main()
{
	FragColor = texture(atlas, TexCoord);
	FragColor.w = transparency;
}