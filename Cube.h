#pragma once

#include "Drawable.h"
#include "Texture.h"

class Cube : public Drawable
{
public:
	Cube(GraphicsD11& gfx);
	~Cube() = default;
	void Update(float delta);

private:
	Texture* tx;
};