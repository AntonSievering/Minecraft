#version 410 core

layout(location = 0) in vec2 a_position;
layout(location = 1) in vec2 a_texCoord;

out vec2 v_texCoord;

uniform vec2 u_panOffset = vec2(0.0f, 0.0f);
uniform float u_fZoom = 1.0f;

vec2 worldToScreen(vec2 pos)
{
	return (pos - u_panOffset) * u_fZoom;
}

void main()
{
	gl_Position = vec4(worldToScreen(a_position), 1.0f, 1.0f);
	v_texCoord = a_texCoord;
}
