#version 330 core

in vec2 texCoord;
in vec3 position;
in vec3 normal;

uniform sampler2D textureSampler;
uniform bool noTexture;

float getLighting()
{
    return clamp (dot(normal, vec3(0.0, 1.0, 0.0)) + 0.5, 0.0f, 1.0f);
}

void setLitTexture()
{
    float lighting = getLighting ();

    if (noTexture)
        gl_FragColor = vec4 (lighting, lighting, lighting, 1.0f);
    else
	    gl_FragColor =	texture(textureSampler, texCoord) * getLighting ();
}

void setUnlitTexture()
{
    gl_FragColor = texture(textureSampler, texCoord);
}