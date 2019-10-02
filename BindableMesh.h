#pragma once

#include "Bindable.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Topology.h"

#include "Mesh.h"

class BindableMesh : public Renderable {

public:
	BindableMesh(GraphicsD11& gfx, Mesh* mesh);

private:
	Mesh* mMesh;
};