#include "BindableMesh.h"

BindableMesh::BindableMesh(GraphicsD11& gfx, Mesh* mesh) {
	
	mMesh = mesh;
	mIndCount = mesh->getNumInds();

	mVertBuf = new VertexBuffer(
		gfx,
		sizeof(Mesh::Vertex) * mesh->getNumVerts(),
		sizeof(Mesh::Vertex),
		mesh->getVerts()
	);

	mIndBuf = new IndexBuffer(
		gfx,
		mesh->getNumInds(),
		mesh->getInds()
	);

	mTopo = new Topology(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}