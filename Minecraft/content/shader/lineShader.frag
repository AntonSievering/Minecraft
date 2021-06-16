#version 330 core

layout(location = 0) out vec4 f_color;

in vec3 v_normal;

uniform vec4 u_color = vec4(0.0f, 0.0f, 0.0f, 1.0f);

void main()
{
	f_color = vec4(u_color.r + abs(v_normal.x), u_color.g + abs(v_normal.y), u_color.b + abs(v_normal.z), u_color.a);
}
