#pragma once

#include "Renderable.h"
#include "Texture.h"
#include "TextureLoader.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Mesh : public Renderable
{
public:
	Mesh(GraphicsD11& gfx, const aiScene* sponzaScene, int index, TextureLoader& loader);
	~Mesh() = default;

	void Bind(GraphicsD11& gfx) override;
	UINT32 mTextureMode = 0;

private:
	Texture* mTextureDiff = nullptr;
	Texture* mTextureSpec = nullptr;
};