#version 330
#extension GL_EXT_gpu_shader4: enable
#extension GL_EXT_geometry_shader4: enable

layout(lines_adjacency) in;
layout(line_strip, max_vertices=200) out;


void main(void)
{
	int p = 30;

	mat4 m;
	m[0] = vec4(-0.5f, 1.5f, -1.5f, 0.5f);
	m[1] = vec4(1.0f, -2.5f, 2.0f, -0.5f);
	m[2] = vec4(-0.5f, 0.0f, 0.5f, 0.0f);
	m[3] = vec4(0.0f, 1.0f, 0.0f, 0.0f);

	vec4 rX, rY;
	rX[0] = gl_in[0].gl_Position.x;
	rX[1] = gl_in[1].gl_Position.x;
	rX[2] = gl_in[2].gl_Position.x;
	rX[3] = gl_in[3].gl_Position.x;

	rY[0] = gl_in[0].gl_Position.y;
	rY[1] = gl_in[1].gl_Position.y;
	rY[2] = gl_in[2].gl_Position.y;
	rY[3] = gl_in[3].gl_Position.y;

	vec4 weightsX = rX * m;
	vec4 weightsY = rY * m;
	
	for(int i=0; i <= p; ++i)
	{
		float t = float(i) / float(p);
		vec4 tvec = vec4(t*t*t, t*t, t, 1.0f);
		vec2 newPoint;
		newPoint.x = dot(tvec, weightsX);
		newPoint.y = dot(tvec, weightsY);
		gl_Position = vec4(newPoint, 0.0f, 1.0f);
		EmitVertex();
	}
	EndPrimitive();
}