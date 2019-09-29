#pragma once

#include "Bindable.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Topology.h"

#include "Mesh.h"

class BindableMesh : public Bindable {

public:
	BindableMesh(GraphicsD11& gfx, Mesh mesh);
	void Bind(GraphicsD11& gfx) noexcept override;

private:
	VertexBuffer* mVertBuffer;
	IndexBuffer* mIndBuffer;
	Topology* mTopo;

};