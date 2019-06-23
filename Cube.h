#pragma once

#include "Drawable.h"

class Cube : public Drawable
{
public:
	Cube(GraphicsD11& gfx);
	~Cube() = default;
};