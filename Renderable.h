#pragma once

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Topology.h"
#include "Transform.h"


class Renderable 
{
public:

	virtual void Bind(GraphicsD11& gfx)
	{
		mVertBuf->Bind(gfx);
		mIndBuf->Bind(gfx);
		mTopo->Bind(gfx);
	}

	UINT GetIndCount()
	{
		return mIndCount;
	}

	const Transform* transform = &mTransform;

protected:
	VertexBuffer* mVertBuf;
	IndexBuffer* mIndBuf;
	Topology* mTopo;

	UINT mIndCount;
	Transform mTransform;
};