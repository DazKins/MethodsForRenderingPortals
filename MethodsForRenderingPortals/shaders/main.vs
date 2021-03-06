#version 330 core

layout (location = 0) in vec3 position_attribute;
layout (location = 1) in vec3 normal_attribute;
layout (location = 2) in vec2 texCoord_attribute;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

out vec3 position;
out vec3 normal;
out vec2 texCoord;

void loadAttributes()
{
    position = position_attribute;
    // normal = vec3 (inverse (transpose (modelMatrix)) * vec4 (normal_attribute, 1.0f));
    normal = normalize (inverse (transpose (mat3 (modelMatrix))) * normal_attribute);
    texCoord = texCoord_attribute;
}

void setStandardPosition()
{
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1.0); 
}