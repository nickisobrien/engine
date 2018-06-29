#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 transform;
uniform mat4 projection;
uniform mat4 view;

out vec3 pos;

void main()
{
	pos = aPos;
	// flattens water levels
	if (pos.z > 5.0f)
		pos.z = 5.0f;
	gl_Position = projection * view * transform * vec4(pos, 1.0);
}