#version 330

in float colorGrade;
out vec4 outColor;



void main(void)
{
	outColor = vec4(0.0f, 0.0f, colorGrade, 1.0f);
}