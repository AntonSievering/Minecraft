#version 330 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_textureCoordinate;

out vec2 v_textureCoordinate;
out float v_fInvGamma;

uniform mat4  u_modelViewProj;
uniform float u_fGamma   = 2.2f;

void main()
{
	gl_Position = u_modelViewProj * vec4(a_position, 1.0f);
	v_textureCoordinate = a_textureCoordinate;
	v_fInvGamma = 1.0f / u_fGamma;
}
