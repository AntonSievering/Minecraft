#version 330 core

layout (location = 0) in vec3 a_pos;

uniform vec3 u_baseCoordinate;
uniform mat4 u_modelViewProj;

void main()
{
	gl_Position = u_modelViewProj * vec4(a_pos + u_baseCoordinate, 1.0f);
}
