#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 textureCoord;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

out vec2 fragmentTextureCoord;
out vec3 position;

void main()
{
	position = aPos;
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(aPos, 1.0);
	fragmentTextureCoord = textureCoord;
}