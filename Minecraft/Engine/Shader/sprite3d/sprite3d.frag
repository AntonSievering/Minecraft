#version 330 core

layout(location = 0) out vec4 f_color;

in vec2 v_textureCoordinate;
in float v_fInvGamma;

uniform sampler2D u_texture;
uniform vec4      u_tint = vec4(1.0f, 1.0f, 1.0f, 1.0f);

void main()
{
	vec3 colour = texture(u_texture, v_textureCoordinate).rgb;
	vec3 vGammaCorrected = pow(colour, vec3(v_fInvGamma));

	f_color = vec4(vGammaCorrected, 1.0f) * u_tint;
}
