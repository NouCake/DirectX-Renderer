#include "Cube.h"

Cube::Cube(GraphicsD11& gfx)
{
	BaseMaterial::VertexInput vi[] =
	{
		{ 0.5f,	 0.5f,	 0.5f}, {1,0,0,1}, //0
		{ 0.5f,	 0.5f,	-0.5f}, {0,1,0,1}, //1
		{-0.5f,	 0.5f,	 0.5f}, {0,0,1,1}, //2
		{-0.5f,	 0.5f,	-0.5f}, {1,0,1,1}, //3
		{ 0.5f,	-0.5f,	 0.5f}, {1,0,0,1}, //4
		{ 0.5f,	-0.5f,	-0.5f}, {0,1,0,1}, //5
		{-0.5f,	-0.5f,	 0.5f}, {0,0,1,1}, //6
		{-0.5f,	-0.5f,	-0.5f}, {1,0,1,1}, //7
	};

	UINT ind[] =
	{
		0, 1, 4,
		0, 4, 1
	};

	mMaterial = new BaseMaterial(gfx);
	mVertBuf = new VertexBuffer(
		gfx,
		sizeof(BaseMaterial::VertexInput) * std::size(vi),
		sizeof(BaseMaterial::VertexInput),
		&vi
	);
	mIndBuf = new IndexBuffer(
		gfx,
		6,
		&ind
	);
	mTopo = new Topology(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}