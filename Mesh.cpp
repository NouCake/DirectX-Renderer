#include "Mesh.h"

#include "NouException.h"
#include "BaseMaterial.h"
#include "NouException.h"

#include <vector>
#include <iostream>

Mesh::Mesh(int numVerts, Vertex::Position* vertPos, Vertex::Normal* vertNorm, 
	Vertex::Tangent* vertTang, Vertex::Bitangent* vertBitang, Vertex::Textcoord* vertTex, int numInds, UINT* indecies) {
	
	mNumVerts = numVerts;
	mVerts = (Vertex*)malloc(sizeof(Vertex) * mNumVerts);
	if (mVerts == nullptr) throw NouException::BaseException(__LINE__, __FILE__, "Malloc failed");

	mHasNormal = vertNorm != nullptr;
	mHasTangent = (vertTang != nullptr) && (vertBitang != nullptr);
	mHasTextCoord = vertTex != nullptr;
	
	Vertex tmp = {};

	int sizePos = sizeof(Vertex::Position);
	int sizeNorm = sizeof(Vertex::Normal);
	int sizeTang = sizeof(Vertex::Tangent);
	int sizeBitang = sizeof(Vertex::Bitangent);
	int sizeTex = sizeof(Vertex::Textcoord);

	for (int i = 0; i < numVerts; i++) {
		void* pCurPos	 = &(mVerts + i)->position;
		memcpy(pCurPos, vertPos + i, sizePos);

		if (mHasNormal) {
			void* pCurNorm = &(mVerts + i)->normal;
			memcpy(pCurNorm, vertNorm + i, sizeNorm);
		}

		if (mHasTangent) {
			void* pCurTang = &(mVerts + i)->tangent;
			void* pCurBitang = &(mVerts + i)->bitangent;
			memcpy(pCurTang, vertTang + i, sizeTang);
			memcpy(pCurBitang, vertBitang + i, sizeBitang);
		}

		if (mHasTextCoord) {
			void* pCurTex = &(mVerts + i)->textcoord;
			memcpy(pCurTex, vertTex + i, sizeTex);
		}
	}

	mNumInds = numInds;
	mInds = (UINT*)malloc(sizeof(UINT) * mNumInds);
	if (mInds == nullptr) throw NouException::BaseException(__LINE__, __FILE__, "Malloc failed");
	
	memcpy((void*)mInds, indecies, sizeof(UINT) * mNumInds);
}

int Mesh::getNumVerts() {
	return mNumVerts;
}

int Mesh::getNumInds() {
	return mNumInds;
}

Mesh::Vertex* Mesh::getVerts() {
	return mVerts;
}

UINT* Mesh::getInds() {
	return mInds;
}