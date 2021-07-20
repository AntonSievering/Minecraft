#version 330 core

layout(location = 0) in uint a_data1;
layout(location = 1) in uint a_data2;

out vec3 v_texCoord;
out float v_brightness;
out vec3 v_color;

uniform float u_fTextureHeight = 1.0f;
uniform mat4 u_modelViewProj;
uniform vec3 u_chunkBaseCoord = vec3(0.0f, 0.0f, 0.0f);

vec2 texCoord[4] = vec2[4](
	vec2(0.0f, 0.0f),
	vec2(1.0f, 0.0f),
	vec2(0.0f, 1.0f),
	vec2(1.0f, 1.0f)
);

float brightness[3] = float[3](1.0f, 0.85f, 0.7f);

void main()
{
	float x = u_chunkBaseCoord.x + float(a_data1 & uint(0x1F));
	float z = u_chunkBaseCoord.z + float((a_data1 & uint(0x3E0)) >> 5);
	float y = u_chunkBaseCoord.y + float(a_data1 >> 12);

	v_brightness = brightness[(a_data1 & uint(0xC00)) >> 10];

	v_texCoord.xy = texCoord[a_data2 & uint(0x3)];
	v_texCoord.z  = (float(a_data2 >> 2) + 0.5f) / u_fTextureHeight;
	
	gl_Position = u_modelViewProj * vec4(x, y, z, 1.0f);
}
