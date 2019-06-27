#pragma once

#include "Texture.h"

#include <map>

class TextureLoader
{
public:
	TextureLoader();

	Texture* GetTexture(GraphicsD11& gfx, std::string path);

private:
	void LoadTexture(GraphicsD11& gfx, std::string path);
	std::map<std::string, Texture> textures;

};