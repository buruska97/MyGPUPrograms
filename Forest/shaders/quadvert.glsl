#version 330

layout(location = 0) in vec2 vPosition;

out float colorGrade;

uniform float pitch;

void main(void)
{
	gl_Position = vec4(vPosition, 1.0f, 1.0f);

	float vertexPitch = pitch + 0.48 * vPosition.y;

	colorGrade = (2 * vertexPitch / 3.1415 + 1) / 2;
}