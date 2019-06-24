#pragma once

#include "Renderable.h"
#include "Texture.h"

class Cube : public Renderable
{
public:
	Cube(GraphicsD11& gfx);
	~Cube() = default;

private:
	Texture* tx;
};