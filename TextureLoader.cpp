#include "TextureLoader.h"

TextureLoader::TextureLoader()
{
}

Texture* TextureLoader::GetTexture(GraphicsD11& gfx, std::string path)
{

	if (textures.find(path) == textures.end())
	{
		LoadTexture(gfx, path);
	}

	return &textures.find(path)->second;
}

void TextureLoader::LoadTexture(GraphicsD11& gfx, std::string path)
{
	textures.insert(std::make_pair(path, Texture(gfx, path)));
}

