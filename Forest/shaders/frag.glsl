#version 330

out vec4 outColor;

in vec3 wView;
in vec3 wNormal;
in vec2 fTexCoord;

uniform sampler2D textureUnit;
uniform vec4 light;
uniform float opacity;

void main(void)
{
	vec4 textureColor = texture(textureUnit, fTexCoord);

	vec3 L = normalize(light.xyz);
	vec3 N = normalize(wNormal);
	vec3 V = normalize(wView);

	vec3 kd = vec3(0.8f, 0.8f, 0.8f);
	vec3 ks = vec3(0.1f, 0.1f, 0.1f);
	vec3 ka = vec3(0.2f, 0.2f, 0.2f);

	vec3 La = vec3(0.5f, 0.5f, 0.5f);
	vec3 Le = vec3(1.0f, 1.0f, 1.0f);

	float shininess = 500.0;
	vec3 H = normalize(L + V);
	float cost = max(dot(N, L), 0.0f);
	float cosd = max(dot(N, H), 0.0f);

	vec3 luminance = ka * La + (kd * textureColor.rgb * cost + ks * pow(cosd, shininess)) * Le;

	outColor = vec4(luminance, opacity);

}