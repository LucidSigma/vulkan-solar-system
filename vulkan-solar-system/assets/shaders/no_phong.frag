#version 450

layout (location = 0) in vec2 v_textureCoordinate;

layout (location = 0) out vec4 out_colour;

layout (set = 0, binding = 1) uniform sampler2D u_textureSampler;

void main()
{
	out_colour = texture(u_textureSampler, v_textureCoordinate);
}