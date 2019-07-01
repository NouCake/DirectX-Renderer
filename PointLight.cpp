#include "PointLight.h"

PointLight::PointLight(float x, float y , float z)
{
	transform = new Glyph3::Transform3D();

	transform->Position().x = x;
	transform->Position().y = y;
	transform->Position().z = z;

	//transform->Rotation().
}