#version 330 core

out vec4 FragColor;

in vec2 texCoord;
in vec3 position;
in vec3 normal;

uniform sampler2D textureSampler;

float getLighting()
{
    return min(dot(normal, vec3(0.0, 1.0, 0.0)) + 0.5, 1.0f);
}

void setLitTexture()
{
	FragColor =	texture(textureSampler, texCoord) * getLighting();
}