uniform vec3 portalPosition;
uniform vec3 portalNormal;

void main()
{
	if (dot(position - portalPosition, portalNormal) < 0)
		discard;

	setLitTexture();
}