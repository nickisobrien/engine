#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Norm;
in float TorchLight;
in float SunLight;
in float Visibility;

// texture sampler
uniform sampler2D atlas;
uniform float transparency;
// uniform vec3 skyColor;
// const vec3 skyColor(0.8f,0.8f,0.9f);


void main()
{
	FragColor = texture(atlas, TexCoord);
	FragColor.w = transparency;

	// pre lighting
	FragColor.x -= 0.5f;
	FragColor.y -= 0.5f;
	FragColor.z -= 0.5f;

	FragColor.x += TorchLight/16;
	FragColor.y += TorchLight/16;
	FragColor.z += TorchLight/16;

	FragColor.x += SunLight/16;
	FragColor.y += SunLight/16;
	FragColor.z += SunLight/16;

	if (Norm.y == -1.0f)
	{
		FragColor.x += 0.2f;
		FragColor.y += 0.2f;
		FragColor.z += 0.2f;
	}
	if (Norm.y == 1.0f)
	{
		FragColor.x -= 0.4f;
		FragColor.y -= 0.4f;
		FragColor.z -= 0.4f;
	}

	// FragColor = mix(vec4(skyColor,1.0 ),FragColor,Visibility);

}