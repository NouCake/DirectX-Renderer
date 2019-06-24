#pragma once

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Topology.h"


class Renderable 
{
public:

	void Bind(GraphicsD11& gfx)
	{
		mVertBuf->Bind(gfx);
		mIndBuf->Bind(gfx);
		mTopo->Bind(gfx);
	}

	void SetPosition(float x, float y, float z)
	{
		ObjectToWorld.r[0].m128_f32[3] = x;
		ObjectToWorld.r[1].m128_f32[3] = y;
		ObjectToWorld.r[2].m128_f32[3] = z;
	}


	UINT GetIndCount()
	{
		return mIndCount;
	}

	DirectX::XMMATRIX ObjectToWorld;

protected:
	VertexBuffer* mVertBuf;
	IndexBuffer* mIndBuf;
	Topology* mTopo;

	UINT mIndCount;
};