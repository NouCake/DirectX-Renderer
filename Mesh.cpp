#include "Mesh.h"

#include "NouException.h"
#include "BaseMaterial.h"

#include <vector>

#include <iostream>

Mesh::Mesh(GraphicsD11& gfx, const aiScene* sponzaScene, int index, TextureLoader& loader)
{
	

	const aiMesh* sponzaMesh = sponzaScene->mMeshes[index];
	mTransform->Scale(0.01f, 0.01f, 0.01f);


	std::vector<BaseMaterial::VertexInput> verts;
	verts.reserve(sponzaMesh->mNumVertices);
	for (unsigned int i = 0; i < sponzaMesh->mNumVertices; i++)
	{
		BaseMaterial::VertexInput vi = {};
		vi.vertex = {
			sponzaMesh->mVertices[i].x,
			sponzaMesh->mVertices[i].y,
			sponzaMesh->mVertices[i].z
		};
		//throw NouException::BaseException(__LINE__, __FILE__, "Vert " + std::to_string(vi.vertex.x) + " " + std::to_string(vi.vertex.y) + " " + std::to_string(vi.vertex.z));
		vi.color = { 255, 255, 255, 255 };
		vi.uv = { 0.5f, 0.5f };
		if (sponzaMesh->HasTextureCoords(0))
		{
			const auto tv = sponzaMesh->mTextureCoords[0][i];
			vi.uv.u = tv.x;
			vi.uv.v = tv.y;
		}
		else
		{
			int estragon = 13;
		}

		if (sponzaMesh->HasNormals())
		{
			vi.normal.x = sponzaMesh->mNormals[i].x;
			vi.normal.y = sponzaMesh->mNormals[i].y;
			vi.normal.z = sponzaMesh->mNormals[i].z;
		}

		verts.push_back(vi);
	}

	//OutputDebugString(("Num of Mesh Verts: " + std::to_string(sponzaMesh->mNumVertices) + "\n").c_str());

	std::vector<UINT16> ind;
	ind.reserve(sponzaMesh->mNumFaces);
	for (unsigned int i = 0; i < sponzaMesh->mNumFaces; i++)
	{
		const auto& face = sponzaMesh->mFaces[i];
		assert(face.mNumIndices == 3);
		ind.push_back(face.mIndices[0]);
		ind.push_back(face.mIndices[1]);
		ind.push_back(face.mIndices[2]);
	}


	mVertBuf = new VertexBuffer(
		gfx,
		sizeof(BaseMaterial::VertexInput) * sponzaMesh->mNumVertices,
		sizeof(BaseMaterial::VertexInput),
		&verts[0]
	);
	mIndBuf = new IndexBuffer(
		gfx,
		sponzaMesh->mNumFaces*3,
		&ind[0]
	);
	mTopo = new Topology(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	int matIndex = sponzaMesh->mMaterialIndex;
	auto mat = sponzaScene->mMaterials[matIndex];
	if (mat->GetTextureCount(aiTextureType_DIFFUSE) > 0)
	{
		aiString astr;
		auto may = mat->GetTexture(aiTextureType_DIFFUSE, 0, &astr);
		mTexture = loader.GetTexture(gfx, "res/" + std::string(astr.C_Str()));
	}
	else {
		int h = 87;
		for (int i = 0; i < mat->mNumProperties; i++)
		{
			auto matProb = mat->mProperties[i];
			const int size = mat->mProperties[i]->mDataLength;
			char* dat = mat->mProperties[i]->mData;
			std::string str = "";
			for (int c = 0; c < size; c++)
			{
				str.push_back(dat[c]);
			}
			h = 0;
			mTexture = loader.GetTexture(gfx, "res/unknown.png");
		}
	}

	if (mat->GetTextureCount(aiTextureType_HEIGHT) > 0)
	{
		int a = 03;
	}

	mIndCount = sponzaMesh->mNumFaces * 3;

	verts.clear();
	ind.clear();

}

void Mesh::Bind(GraphicsD11& gfx)
{
	mVertBuf->Bind(gfx);
	mIndBuf->Bind(gfx);
	mTopo->Bind(gfx);
	if (this->mTexture != nullptr) {

		this->mTexture->Bind(gfx);

	}
}