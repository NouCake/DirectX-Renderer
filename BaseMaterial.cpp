#include "BaseMaterial.h"
#include "Transform.h"
#include "Mesh.h"
#include "ImGUI/imgui.h"

BaseMaterial::BaseMaterial(GraphicsD11& gfx) {

	mPathVert = L"vertex.cso";
	mPathFrag = L"frag.cso";

	mVertUniformLength = sizeof(VertexUniforms);
	mFragUniformLength = sizeof(FragmentUniforms);
 
	mCurFragUniforms = {};
	mCurFragUniforms.LightIntensity = 1.0f;
	mCurFragUniforms.LightPos[0] = 0.0f;

	mCurVertexUniforms = {};
	mCurVertexUniforms.WorldToView = DirectX::XMMatrixTranslation(2.0f, 0.0f, 5.0f);
	mCurVertexUniforms.WorldToView *= DirectX::XMMatrixPerspectiveFovLH(45.0f, 720.0f / 480.0f, 0.1f, 100.f);  //Projection
	mCurVertexUniforms.WorldToView = DirectX::XMMatrixTranspose(mCurVertexUniforms.WorldToView);
	//mCurVertexUniforms.WorldToView = DirectX::XMMatrixIdentity();
	mCurVertexUniforms.ObjectToWorld = DirectX::XMMatrixIdentity();

	unsigned int offs = 0;
	mLayout = {
		{"Position",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"Normal",		0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offs += sizeof(Mesh::Vertex::Position), D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"Tangent",		0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offs += sizeof(Mesh::Vertex::Normal), D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"Bitangent",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offs += sizeof(Mesh::Vertex::Tangent), D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"UV",			0, DXGI_FORMAT_R32G32_FLOAT,	0, offs += sizeof(Mesh::Vertex::Bitangent), D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	Setup(gfx);

}

void BaseMaterial::Begin(GraphicsD11& gfx, Camera& cam) {
	mFragShader->Bind(gfx);
	mVertShader->Bind(gfx);
	mInputLayout->Bind(gfx);
	mVertCB->Bind(gfx);
	mFragCB->Bind(gfx);

	//mCurVertexUniforms.CamPos = cam.mPos;
	//mCurVertexUniforms.WorldToView = cam.GetMatrix(); 
}

void BaseMaterial::UpdateUniforms(GraphicsD11& gfx, Renderable& rend) {

	mVertCB->Update(gfx, sizeof(VertexUniforms), &mCurVertexUniforms);
	mFragCB->Update(gfx, sizeof(FragmentUniforms), &mCurFragUniforms);
}

void BaseMaterial::Draw(GraphicsD11& gfx, Renderable& rend) {
	UpdateUniforms(gfx, rend);
	rend.Bind(gfx);
	gfx.DrawIndexed(rend.GetIndCount());
}