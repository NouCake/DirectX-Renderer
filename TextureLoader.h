#pragma once

#include "Texture.h"

#include <vector>

class TextureLoader
{
public:
	TextureLoader();

	Texture* GetTexture(GraphicsD11& gfx, std::string path);

private:
	void LoadTexture(GraphicsD11& gfx, std::string path);
	std::vector<Texture*> textures;

};