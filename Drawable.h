#pragma once

#include "BaseMaterial.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Topology.h"


class Drawable
{
public:

	void SetMaterial(Material* mat)
	{
		mMaterial = mat;
	}

	void SetPosition(float x, float y, float z)
	{
		transform.r[0].m128_f32[3] = x;
		transform.r[1].m128_f32[3] = y;
		transform.r[2].m128_f32[3] = z;
	}

	DirectX::XMMATRIX transform;

protected:
	Material* mMaterial;
	VertexBuffer* mVertBuf;
	IndexBuffer* mIndBuf;
	Topology* mTopo;

	UINT mIndCount;
};