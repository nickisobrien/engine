#version 330 core
out vec4 FragColor;

in vec3 TexCoord;
in vec3 pos;

// texture sampler
uniform sampler2D grassTexture;
uniform sampler2D sandTexture;
uniform sampler2D snowTexture;

void main()
{
	if (TexCoord.z == 1)
		FragColor = texture(grassTexture, vec2(TexCoord.x, TexCoord.y));
	else if (TexCoord.z == 2)
		FragColor = texture(sandTexture, vec2(TexCoord.x, TexCoord.y));
	else
		FragColor = texture(snowTexture, vec2(TexCoord.x, TexCoord.y));
	// if (pos.y <= 60) // can take out comparison opperator by making a simple array of textures and doing texture[0]+TexCoord.z
	// 	FragColor =  texture(sandTexture, vec2(TexCoord.x, TexCoord.y));
	// else if (pos.y <= 80)
	// 	FragColor = texture(grassTexture, vec2(TexCoord.x, TexCoord.y));
	// else
	// 	FragColor = texture(snowTexture, vec2(TexCoord.x, TexCoord.y));
}