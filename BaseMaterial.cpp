#include "BaseMaterial.h"

BaseMaterial::BaseMaterial(GraphicsD11& gfx)
{

	mVertShader = new VertexShader(gfx, L"vertex.cso");
	mFragShader = new FragmentShader(gfx, L"frag.cso");

	mVertCB = new VertexConstantBuffer(gfx, sizeof(VertexUniforms), &DirectX::XMMatrixIdentity());

	std::vector<D3D11_INPUT_ELEMENT_DESC> layout = {
		{"Position", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"Color", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, sizeof(BaseMaterial::VertexInput::Vertex), D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	mInputLayout = new InputLayout(gfx, layout, mVertShader->GetBytecode());
}

void BaseMaterial::Begin(GraphicsD11& gfx)
{
	mVertShader->Bind(gfx);
	mFragShader->Bind(gfx);
	mInputLayout->Bind(gfx);
	mVertCB->Bind(gfx);
}

void BaseMaterial::Draw(GraphicsD11& gfx)
{
	//UpdateUniforms

	//UpdateVertexBuffer
	//UpdateIndexBuffer
	//UpdateTopology 
	//Draw Triangles
}