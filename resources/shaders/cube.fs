#version 330 core
out vec4 FragColor;

in vec3 TexCoord;
in vec3 pos;

// texture sampler
uniform sampler2D grassTexture;
uniform sampler2D sandTexture;

void main()
{
	if (TexCoord.z == 1) // can take out comparison opperator by making a simple array of textures and doing texture[0]+TexCoord.z
		FragColor =  texture(grassTexture, vec2(TexCoord.x, TexCoord.y));
	else
		FragColor = texture(sandTexture, vec2(TexCoord.x, TexCoord.y));
}