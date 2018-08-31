#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNorm;
layout (location = 3) in float aTorchLight;
layout (location = 4) in float aSunLight;

uniform mat4 transform;
uniform mat4 projection;
uniform mat4 view;

out vec2 TexCoord;
out vec3 Norm;
out float TorchLight;
out float SunLight;
// out float Visibility;

// const float density = 0.007f;
// const float gradient = 1.5f;

void main()
{
	SunLight = aSunLight;
	TorchLight = aTorchLight;
	TexCoord = aTexCoord;
	Norm = aNorm;

	// vec4 positionRelativeToCam = view * transform;
	gl_Position = projection * view * transform * vec4(aPos, 1.0f);

	// float dist = length(positionRelativeToCam.xyz);
	// Visibility = exp(-pow((dist*density),gradient));
	// Visibility = clamp(Visibility,0.0,1.0);
}