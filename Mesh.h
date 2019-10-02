#pragma once

#include "Renderable.h"
#include "Texture.h"
#include "TextureLoader.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Mesh
{
public:
	struct Vertex {
		struct Position {
			float x;
			float y;
			float z;
		} position;

		struct Normal {
			float x;
			float y;
			float z;
		} normal;

		struct Tangent {
			float x;
			float y;
			float z;
		} tangent;

		struct Bitangent {
			float x;
			float y;
			float z;
		} bitangent;

		struct Textcoord {
			float u;
			float v;
		} textcoord;
	};

public:
	Mesh(int numVerts, Vertex::Position* vertPos, Vertex::Normal* vertNorm,
		Vertex::Tangent* vertTang, Vertex::Bitangent* vertBitang, 
		Vertex::Textcoord* vertTex, int numInds, UINT16* indecies);
	~Mesh() = default;

	int getNumVerts();
	int getNumInds();

	Vertex* getVerts();
	UINT16* getInds();

private:

	int mNumVerts;
	int mNumInds;

	Vertex* mVerts;
	UINT16* mInds;

	bool mHasNormal;
	bool mHasTangent;
	bool mHasTextCoord;

};