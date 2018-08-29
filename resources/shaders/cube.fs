#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Norm;
in float TorchLight;

// texture sampler
uniform sampler2D atlas;
uniform float transparency;

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

	if (Norm.y == -1.0f)
	{
		FragColor.x += 0.1f;
		FragColor.y += 0.1f;
		FragColor.z += 0.1f;
	}
	if (Norm.y == 1.0f)
	{
		FragColor.x -= 0.1f;
		FragColor.y -= 0.1f;
		FragColor.z -= 0.1f;
	}

}