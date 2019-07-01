#include "BaseMaterial.h"
#include "Transform.h"
#include "Mesh.h"

#include "Hieroglyph/Vector3f.h"

#include "ImGUI/imgui.h"

BaseMaterial::BaseMaterial(GraphicsD11& gfx)
{

	mVertShader = new VertexShader(gfx, L"vertex.cso");
	mFragShader = new FragmentShader(gfx, L"frag.cso");

	mCurVertexUniforms = {};
	mCurFragUniforms = {};
	mCurFragUniforms.LightIntensity = 1.0f;
	mCurFragUniforms.LightPos[0] = 0.0f;
	//mCurFragUniforms.LightPos[1] = 0.0f;
	mCurVertexUniforms.WorldToView = DirectX::XMMatrixTranslation(0.0f, 0.0f, 5.0f) *  // -CamPos
		DirectX::XMMatrixPerspectiveFovLH(45.0f, 720.0f / 480.0f, 0.1f, 100.f);  //Projection
	mCurVertexUniforms.WorldToView = DirectX::XMMatrixTranspose(mCurVertexUniforms.WorldToView);
	mCurVertexUniforms.ObjectToWorld = DirectX::XMMatrixIdentity();
	mVertCB = new VertexConstantBuffer(gfx, sizeof(VertexUniforms), &mCurVertexUniforms);
	mFragCB = new FragmentConstantbuffer(gfx, sizeof(FragmentUniforms), &mCurFragUniforms);

	unsigned int offs = 0;
	std::vector<D3D11_INPUT_ELEMENT_DESC> layout = {
		{"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"Color", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, (offs += sizeof(BaseMaterial::VertexInput::Vertex)), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, (offs += sizeof(BaseMaterial::VertexInput::Color)), D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"Normal", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, (offs += sizeof(BaseMaterial::VertexInput::UV)), D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	mInputLayout = new InputLayout(gfx, layout, mVertShader->GetBytecode());


	gTM = new bool[3];
	gTM[0] = true;
	gTM[1] = true;
	gTM[2] = true;
}

void BaseMaterial::Begin(GraphicsD11& gfx, Camera& cam)
{
	mFragShader->Bind(gfx);
	mVertShader->Bind(gfx);
	mInputLayout->Bind(gfx);
	mVertCB->Bind(gfx);
	mFragCB->Bind(gfx);

	mCurVertexUniforms.CamPos = cam.mPos;
	mCurVertexUniforms.WorldToView = cam.GetMatrix();

	ImGui::Checkbox("Diffuse", gTM + 0);
	ImGui::Checkbox("Specula", gTM + 1);
	ImGui::Checkbox("Ambient", gTM + 2);
	ImGui::SliderFloat("LightIntensity", &mCurFragUniforms.LightIntensity, 0.0f, 2.0f);
	ImGui::SliderFloat2("LightPosition", &mCurFragUniforms.LightPos[0], -10.0f, 10.0f);
	
	UINT32 mask = 0;
	mask += *(gTM + 0) ? 0 : TEXTUREMODE_DIFFUSE;
	mask += *(gTM + 1) ? 0 : TEXTUREMODE_SPECULAR;
	mask += *(gTM + 2) ? 0 : TEXTUREMODE_AMBIENT;
	mCurFragUniforms.TextureMode = mask;

	if (ImGui::Button("Stop"))
	{
		UINT nem = 10 & mask;
		int none = 0;
	}

}

void BaseMaterial::UpdateUniforms(GraphicsD11& gfx, Renderable& rend)
{

	mCurVertexUniforms.ObjectToWorld = dx::XMMatrixTranspose(rend.GetTransform().GetLocalTransform());
	mVertCB->Update(gfx, sizeof(VertexUniforms), &mCurVertexUniforms);


	mCurFragUniforms.TextureMode = 0;
	if (Mesh * m = dynamic_cast<Mesh*>(&rend))
	{
		UINT32 mask = 0xffffffff;
		mask -= *(gTM + 0) ? 0 : TEXTUREMODE_DIFFUSE & m->mTextureMode;
		mask -= *(gTM + 1) ? 0 : TEXTUREMODE_SPECULAR & m->mTextureMode;
		mask -= *(gTM + 2) ? 0 : TEXTUREMODE_AMBIENT & 0xffffffff;

		mCurFragUniforms.TextureMode = mask & m->mTextureMode;
	}

	mFragCB->Update(gfx, sizeof(FragmentUniforms), &mCurFragUniforms);
}

void BaseMaterial::Draw(GraphicsD11& gfx, Renderable& rend)
{
	UpdateUniforms(gfx, rend);
	rend.Bind(gfx);
	gfx.DrawIndexed(rend.GetIndCount());
}

void BaseMaterial::SetTextureMode(UINT32 TextureMode)
{
	//mCurFragUniforms.TextureMode = TextureMode;
}