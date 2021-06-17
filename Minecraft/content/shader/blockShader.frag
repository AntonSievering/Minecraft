#version 330 core

layout(location = 0) out vec4 f_color;

in vec3 v_texCoord;
in float v_brightness;

uniform sampler3D u_texture;

void main()
{
	f_color	= textureLod(u_texture, v_texCoord, 0) * vec4(v_brightness, v_brightness, v_brightness, 1.0f);
}
