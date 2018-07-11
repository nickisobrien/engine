#version 330 core
out vec4 FragColor;

in vec3 TexCoord;
in vec3 pos;

// texture sampler
uniform sampler2D grassTexture;
uniform sampler2D sandTexture;

void main()
{
	if (TexCoord.z == 1)
		FragColor =  texture(grassTexture, vec2(TexCoord.x, TexCoord.y));
	else
		FragColor = texture(sandTexture, vec2(TexCoord.x, TexCoord.y));
}