#version 450

layout (location = 0) in vec3 v_textureCoordinates;

layout (location = 0) out vec4 out_colour;

layout (set = 0, binding = 1) uniform samplerCube u_samplerCube;

void main()
{
	out_colour = texture(u_samplerCube, v_textureCoordinates);
}