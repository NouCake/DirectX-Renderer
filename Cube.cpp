#include "Cube.h"

#include "ImGUI/imgui.h"

Cube::Cube(GraphicsD11& gfx)
{
	BaseMaterial::VertexInput vi[] = {
		{ {-0.5f,	 0.5f,	 0.5f}, {255,000,000,255}, {0.0f,	0.25f}},  //0
		{ {-0.5f,	-0.5f,	 0.5f}, {255,255,000,255}, {0.0f,	0.5f}}, //1
		{ {-0.5f,	 0.5f,	 0.5f}, {255,255,000,255}, {0.25f,	0.0f}},  //2
		{ {-0.5f,	 0.5f,	-0.5f}, {255,000,000,255}, {0.25f,	0.25f}}, //3
		{ {-0.5f,	-0.5f,	-0.5f}, {255,255,000,255}, {0.25f,	0.5f}},  //4
		{ {-0.5f,	-0.5f,	 0.5f}, {255,255,000,255}, {0.25f,	0.75f}}, //5
		{ { 0.5f,	 0.5f,	 0.5f}, {255,000,000,255}, {0.5f,	0.0f}},  //6
		{ { 0.5f,	 0.5f,	-0.5f}, {255,255,000,255}, {0.5f,	0.25f}}, //7
		{ { 0.5f,	-0.5f,	-0.5f}, {255,255,000,255}, {0.5f,	0.5f}},  //8
		{ { 0.5f,	-0.5f,	 0.5f}, {255,255,000,255}, {0.5f,	0.75f}}, //9
		{ { 0.5f,	 0.5f,	 0.5f}, {255,255,000,255}, {0.75f,	0.25f}}, //10
		{ { 0.5f,	-0.5f,	 0.5f}, {255,255,000,255}, {0.75f,	0.5f}},  //11
		{ {-0.5f,	 0.5f,	 0.5f}, {255,255,000,255}, {1.0f,	0.25f}}, //12
		{ {-0.5f,	-0.5f,	 0.5f}, {255,255,000,255}, {1.0f,	0.5f}},  //13
	};

	UINT16 ind[] = {
		
		0, 3, 1,
		3, 4, 1,
		2, 6, 3,
		6, 7, 3,
		3, 7, 4,
		7, 8, 4,
		3, 7, 4,
		7, 8, 4,
		4, 8, 5,
		8, 9, 5,
		7, 10, 8,
		10, 11, 8,
		10, 12, 11,
		12, 13, 11
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
		std::size(ind),
		&ind
	);
	mTopo = new Topology(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	tx = new Texture(gfx);
	tx->Bind(gfx);

	mIndCount = std::size(ind);
}

void Cube::Update(float delta)
{
	static float t;
	t += delta;
	static float x = 1;
	ImGui::SliderFloat("posX", &x, -3.0f, 3.0f);

	BaseMaterial::VertexUniforms vu;
	vu.transform = DirectX::XMMatrixRotationY(2 * t * 3.1415f * 2) * DirectX::XMMatrixRotationX(t);
	vu.transform *= DirectX::XMMatrixTranslation(x, 0.0f, 5.0f) * DirectX::XMMatrixPerspectiveFovLH(45.0f, 720.0f / 480.0f, 0.1f, 100.f);
	vu.transform = DirectX::XMMatrixTranspose(vu.transform);
	((BaseMaterial*)mMaterial)->UpdateUniforms(vu);

}