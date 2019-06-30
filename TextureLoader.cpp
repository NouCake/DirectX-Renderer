#include "TextureLoader.h"
#include "NouTimer.h"

TextureLoader::TextureLoader()
{
}

Texture* TextureLoader::GetTexture(GraphicsD11& gfx, std::string path)
{

	Texture* ptr = nullptr;
	for (int i = 0; i < textures.size(); i++)
	{
		if (textures[i]->path == path)
		{
			ptr = textures[i];

			return ptr;
		}
	}

	if (ptr == nullptr)
	{
		static NouTimer* timer = new NouTimer();
		timer->Mark();
		LoadTexture(gfx, path);
		OutputDebugString(("Loading " + path + " took: " + std::to_string(timer->Peek()) + "\n").c_str());
		return textures[textures.size() - 1];
	}

	return nullptr;
}

void TextureLoader::LoadTexture(GraphicsD11& gfx, std::string path)
{
	textures.push_back(new Texture(gfx, path));
}

