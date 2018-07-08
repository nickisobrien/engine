#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

struct DirLight {
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);

uniform mat4 transform;
uniform mat4 projection;
uniform mat4 view;

out vec2 TexCoord;
out vec3 pos;

void main()
{
	pos = aPos;
	gl_Position = projection * view * transform * vec4(aPos, 1.0);
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}