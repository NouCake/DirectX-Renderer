#pragma once

#include "Renderable.h"
#include "Texture.h"

class Mesh : public Renderable
{
public:
	Mesh(GraphicsD11& gfx, std::string path, int index);
	~Mesh() = default;

	void Bind(GraphicsD11& gfx) override;

private:
	Texture* mTexture;
};