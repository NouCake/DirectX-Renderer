#pragma once

class PointLight
{
public:
	PointLight(float x, float y, float z);
	~PointLight() = default;

private:
	float mIntensity = 1.0f;
};