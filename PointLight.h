#pragma once

#include "Hieroglyph/Transform3D.h"

class PointLight
{
public:
	PointLight(float x, float y, float z);
	~PointLight() = default;

private:
	float mIntensity = 1.0f;
	Glyph3::Transform3D* transform;
};