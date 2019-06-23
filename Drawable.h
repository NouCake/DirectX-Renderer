#pragma once

#include "BaseMaterial.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Topology.h"
class Drawable
{
public:
	void Draw(GraphicsD11& gfx)
	{
		mMaterial->Begin(gfx);

		mVertBuf->Bind(gfx);
		mIndBuf->Bind(gfx);
		mTopo->Bind(gfx);
	}

protected:
	Material* mMaterial;
	VertexBuffer* mVertBuf;
	IndexBuffer* mIndBuf;
	Topology* mTopo;
};