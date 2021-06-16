#version 410 core

layout(location = 0) out vec4 f_color;

in vec2 v_texCoord;

uniform sampler2D u_texture;
uniform vec4 u_color;

void main()
{
	f_color = texture(u_texture, v_texCoord) * u_color;
}
