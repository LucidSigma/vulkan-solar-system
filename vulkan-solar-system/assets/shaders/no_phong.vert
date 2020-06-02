#version 450

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec2 in_textureCoordinate;

layout (location = 0) out vec2 v_textureCoordinate;

layout (std140, push_constant) uniform Model
{
	mat4 model;
} u_Model;

layout (std140, set = 0, binding = 0) uniform VP
{
	mat4 view;
	mat4 projection;
} u_VP;

void main()
{
	v_textureCoordinate = in_textureCoordinate;

	gl_Position = u_VP.projection * u_VP.view * u_Model.model * vec4(in_position, 1.0);
}