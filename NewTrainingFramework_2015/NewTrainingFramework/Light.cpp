#include "stdafx.h"
#include "Light.h"


Light::Light()
{

}


Light::~Light()
{

}

void Light::setId(int i)
{
	id = i;
}

void Light::setDiffusal(Vector3 dif)
{
	diffusal = dif;
}

void Light::setSpecular(Vector3 spec)
{
	specular = spec;
}

int Light::getId()
{
	return id;
}

Vector3 Light::getDiffusal()
{
	return diffusal;
}

Vector3 Light::getSpecular()
{
	return specular;
}
