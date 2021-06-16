#version 330 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_texCoord;
layout(location = 2) in float a_brightness;

out vec3 v_texCoord;
out float v_brightness;

uniform mat4 u_modelViewProj;

void main()
{
	v_brightness = a_brightness;
	v_texCoord = a_texCoord;
	gl_Position = u_modelViewProj * round(vec4(a_position, 1.0f));
}
