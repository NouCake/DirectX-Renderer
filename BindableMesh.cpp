#include "BindableMesh.h"

BindableMesh::BindableMesh(GraphicsD11& gfx, Mesh mesh) {
	
	mVertBuffer = new VertexBuffer(
		gfx,
		sizeof(Mesh::Vertex) * mesh.getNumVerts(),
		sizeof(Mesh::Vertex),
		mesh.getVerts()
	);

	mIndBuffer = new IndexBuffer(
		gfx,
		mesh.getNumInds(),
		mesh.getInds()
	);

	mTopo = new Topology(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void BindableMesh::Bind(GraphicsD11& gfx) noexcept {
	mVertBuffer->Bind(gfx);
	mIndBuffer->Bind(gfx);
	mTopo->Bind(gfx);
}