#version 330 core

out vec4 FragColor;

in vec2 fragmentTextureCoord;
in vec3 position;

uniform sampler2D textureSampler;

uniform vec3 portalPosition;
uniform vec3 portalNormal;

void main()
{
	if (dot(position - portalPosition, portalNormal) < 0)
		discard;

	FragColor =	texture(textureSampler, fragmentTextureCoord);
}