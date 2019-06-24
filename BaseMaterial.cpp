#include "BaseMaterial.h"

BaseMaterial::BaseMaterial(GraphicsD11& gfx)
{

	mVertShader = new VertexShader(gfx, L"vertex.cso");
	mFragShader = new FragmentShader(gfx, L"frag.cso");

	mCurUniforms = {};
	mCurUniforms.transform = DirectX::XMMatrixTranslation(0.0f, 0.0f, 1.0f) * DirectX::XMMatrixPerspectiveFovLH(45.0f, 720.0f / 480.0f, 0.1f, 100.f);
	mCurUniforms.transform = DirectX::XMMatrixTranspose(mCurUniforms.transform);
	mVertCB = new VertexConstantBuffer(gfx, sizeof(VertexUniforms), &mCurUniforms);

	std::vector<D3D11_INPUT_ELEMENT_DESC> layout = {
		{"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"Color", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, sizeof(BaseMaterial::VertexInput::Vertex), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(BaseMaterial::VertexInput::Vertex) + sizeof(BaseMaterial::VertexInput::Color), D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	mInputLayout = new InputLayout(gfx, layout, mVertShader->GetBytecode());
}

void BaseMaterial::Begin(GraphicsD11& gfx)
{
	mFragShader->Bind(gfx);
	mVertShader->Bind(gfx);
	mInputLayout->Bind(gfx);
	mVertCB->Bind(gfx);

	mVertCB->Update(gfx, sizeof(VertexUniforms), &mCurUniforms);
}

void BaseMaterial::UpdateUniforms(VertexUniforms uniform)
{
	mCurUniforms = uniform;
}

void BaseMaterial::Draw(GraphicsD11& gfx)
{
}