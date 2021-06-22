#version 330 core

layout(location = 0) out vec4 f_color;

uniform vec4 u_color = vec4(0.0f, 0.0f, 0.0f, 1.0f);

void main()
{
	f_color = vec4(u_color.r, u_color.g, u_color.b, u_color.a);
}
