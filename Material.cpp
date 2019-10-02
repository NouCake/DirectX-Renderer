#include "Material.h"

void Material::Setup(GraphicsD11& gfx) {
	LoadShader(gfx);
	InitUniforms(gfx);

	mInputLayout = new InputLayout(gfx, mLayout, mVertShader->GetBytecode());
}

void Material::LoadShader(GraphicsD11& gfx) {

	if (mVertShader != nullptr || mFragShader != nullptr) {
		delete mVertShader;
		delete mFragShader;
	}

	mVertShader = new VertexShader(gfx, mPathVert);
	mFragShader = new FragmentShader(gfx, mPathFrag);

}

void Material::Begin(GraphicsD11& gfx, Camera& cam) {
	mFragShader->Bind(gfx);
	mVertShader->Bind(gfx);
	mInputLayout->Bind(gfx);
	mVertCB->Bind(gfx);
	mFragCB->Bind(gfx);
}

void Material::InitUniforms(GraphicsD11& gfx) {

	int len = mVertUniformLength > mFragUniformLength ? mVertUniformLength : mFragUniformLength;
	void* ntrl = malloc(len);
	if (ntrl == nullptr) throw NouException::BaseException(__LINE__, __FILE__, "nullptr wtf");
	memset(ntrl, 0, len);

	mVertCB = new VertexConstantBuffer(gfx, mVertUniformLength, ntrl);
	mFragCB = new FragmentConstantbuffer(gfx, mFragUniformLength, ntrl);
	free(ntrl);
}