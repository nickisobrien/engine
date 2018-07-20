#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 pos;

// texture sampler
uniform sampler2D atlas;
// uniform sampler2D sandTexture;
// uniform sampler2D snowTexture;
// uniform sampler2D waterTexture;

void main()
{
	// if (TexCoord.z == 1 && TexCoord.z < 2)
	// 	FragColor = texture(grassTexture, vec2(TexCoord.x, TexCoord.y));
	// else if (TexCoord.z == 2 && TexCoord.z < 3)
	// 	FragColor = texture(sandTexture, vec2(TexCoord.x, TexCoord.y));
	// else if (TexCoord.z == 3 && TexCoord.z < 4)
	// 	FragColor = texture(snowTexture, vec2(TexCoord.x, TexCoord.y));
	// else
	// 	FragColor = texture(waterTexture, vec2(TexCoord.x, TexCoord.y));

	FragColor = texture(atlas, TexCoord);


	// if (pos.y <= 60) // can take out comparison opperator by making a simple array of textures and doing texture[0]+TexCoord.z
	// 	FragColor =  texture(sandTexture, vec2(TexCoord.x, TexCoord.y));
	// else if (pos.y <= 80)
	// 	FragColor = texture(grassTexture, vec2(TexCoord.x, TexCoord.y));
	// else
	// 	FragColor = texture(snowTexture, vec2(TexCoord.x, TexCoord.y));
}