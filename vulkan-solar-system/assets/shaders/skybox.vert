#version 450

layout (location = 0) in vec3 in_position;

layout (location = 0) out vec3 v_textureCoordinates;

layout (std140, set = 0, binding = 0) uniform MP
{
	mat4 modelView;
	mat4 projection;
} u_MP;

void main()
{
	v_textureCoordinates = in_position;
	v_textureCoordinates.x *= -1.0;

	gl_Position = u_MP.projection * u_MP.modelView * vec4(in_position, 1.0);
}