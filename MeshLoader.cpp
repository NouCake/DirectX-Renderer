#include "MeshLoader.h"

#include "Mesh.h"
#include "NouTimer.h"
#include "NouException.h"

MeshLoader::MeshLoader() {

}

void MeshLoader::loadMesh(std::string path) {

	NouTimer* timer = new NouTimer();
	timer->Mark();

	const aiScene* scene = mAssimp.ReadFile(path,
		aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_FlipUVs);

	if (scene == nullptr) {
		throw NouException::BaseException(__LINE__, __FILE__, "could not find File " + path);
	}

	int numMeshes = scene->mNumMeshes;
	int curMeshIndex = 0;
	const aiMesh* curMesh = scene->mMeshes[curMeshIndex];


	const int numVerts = curMesh->mNumVertices;

	Mesh::Vertex::Position* pPos = nullptr;
	if (curMesh->HasPositions()) {
		assert(sizeof(Mesh::Vertex::Position) == sizeof(aiVector3D));
		pPos = (Mesh::Vertex::Position*)(&curMesh->mVertices[0]);
	}

	Mesh::Vertex::Normal* pNorm = nullptr;
	if (curMesh->HasNormals()) {
		assert(sizeof(Mesh::Vertex::Normal) == sizeof(aiVector3D));
		pNorm = (Mesh::Vertex::Normal*)(&curMesh->mNormals[0]);
	}

	Mesh::Vertex::Tangent* pTang = nullptr;
	Mesh::Vertex::Bitangent* pBitang = nullptr;
	if (curMesh->HasTangentsAndBitangents()) {
		assert(sizeof(Mesh::Vertex::Tangent) == sizeof(aiVector3D));
		assert(sizeof(Mesh::Vertex::Bitangent) == sizeof(aiVector3D));
		pTang = (Mesh::Vertex::Tangent*)(&curMesh->mTangents[0]);
		pBitang = (Mesh::Vertex::Bitangent*)(&curMesh->mBitangents[0]);
	}

	Mesh::Vertex::Textcoord* pUV = nullptr;
	if (curMesh->HasTextureCoords(0)) {
		assert(sizeof(Mesh::Vertex::Textcoord) == sizeof(aiVector3D));
		
		std::vector<Mesh::Vertex::Textcoord> uvs; //TODO: Does this run out of scope? :^(
		uvs.reserve(numVerts);
		for (int i = 0; i < numVerts; i++) {
			uvs.push_back({curMesh->mTextureCoords[0][i].x, curMesh->mTextureCoords[0][i].y});
		}

		pUV = uvs.data();
	}

	int numInds = curMesh->mNumFaces * 3;
	UINT16* pInds = nullptr;

	std::vector<UINT16> inds;
	inds.reserve(curMesh->mNumFaces*3);
	for (int i = 0; i < curMesh->mNumFaces; i++) {
		inds.push_back(curMesh->mFaces[i].mIndices[0]);
		inds.push_back(curMesh->mFaces[i].mIndices[1]);
		inds.push_back(curMesh->mFaces[i].mIndices[2]);
	}
	pInds = (UINT16*)inds.data();

	Mesh m(numVerts, pPos, pNorm, pTang, pBitang, pUV, numInds, pInds);
	//uvs.clear();
	inds.clear();

	OutputDebugString(("Loading " + path + " took: " + std::to_string(timer->Peek()) + "\n").c_str());
}