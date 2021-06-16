#version 330 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;

out vec3 v_normal;

uniform mat4 u_modelViewProj;

void main()
{
	gl_Position = u_modelViewProj * vec4(a_pos, 1.0f);
	v_normal = a_normal;
}
