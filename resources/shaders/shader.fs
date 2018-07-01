#version 330 core
out vec4 FragColor;

in vec3 pos;
uniform samplerCube skybox;

void main()
{
	// FragColor = texture(skybox, pos);
	// set colors for grid
	if (pos.z < 5.0f && pos.z > 0.0f)
		FragColor = vec4(1.0f, 1.0f, 0.2f, 1.0f);
	else if (pos.z < 0.0f && pos.z > -6.0f)
		FragColor = vec4(0.1f, 1.0f, 0.1f, 1.0f);
	else if (pos.z < -6.0f)
		FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	else
		FragColor = vec4(0.2f, 0.2f, 1.0f, 1.0f);
}