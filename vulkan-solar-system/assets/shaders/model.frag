#version 450

layout (location = 0) in vec2 v_textureCoordinate;
layout (location = 1) in vec3 v_normal;
layout (location = 2) in vec3 v_fragmentPosition;

layout (location = 0) out vec4 out_colour;

layout (set = 0, binding = 1) uniform sampler2D u_textureSampler;

layout (set = 0, binding = 2) uniform Viewer
{
	vec3 position;
} u_Viewer;

void main()
{
	const vec3 LIGHT_COLOUR = vec3(1.0, 1.0, 1.0);

	// Ambient lighting.
	const float AMBIENT_STRENGTH = 0.02;
	const vec3 ambientLight = AMBIENT_STRENGTH * LIGHT_COLOUR;

	// Diffuse lighting.
	const vec3 SUN_POSITION = vec3(0.0, 0.0, 0.0);

	const vec3 lightDirection = normalize(SUN_POSITION - v_fragmentPosition);
	const float diffuseStrength = max(dot(v_normal, lightDirection), 0.0);
	const vec3 diffuseLight = diffuseStrength * LIGHT_COLOUR;

	// Specular lighting.
	const float SPECULAR_STRENGTH = 0.3;
	const vec3 viewDirection = normalize(u_Viewer.position - v_fragmentPosition);
	const vec3 reflectDirection = reflect(-lightDirection, v_normal);
	const float specularPower = pow(max(dot(reflectDirection, viewDirection), 0.0), 16);
	const vec3 specularLight = SPECULAR_STRENGTH * specularPower * LIGHT_COLOUR;

	// Final result.
	out_colour = vec4(ambientLight + diffuseLight + specularLight, 1.0) * texture(u_textureSampler, v_textureCoordinate);
}