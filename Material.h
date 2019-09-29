#pragma once

#include "Camera.h"
#include "FragmentShader.h"
#include "VertexShader.h"
#include "InputLayout.h"
#include "ConstantBuffer.h"
#include "Renderable.h"

class Material
{
public:
	Material(GraphicsD11& gfx);
	~Material() = default;
	virtual void Draw(GraphicsD11& gfx, Renderable& rend) = 0;
	virtual void Begin(GraphicsD11& gfx, Camera& cam) = 0;

	void LoadShader(GraphicsD11& gfx);

protected:

	std::wstring mPathVert;
	std::wstring mPathFrag;
	std::vector<D3D11_INPUT_ELEMENT_DESC> mLayout;

	int mVertUniformLength;
	int mFragUniformLength;

	VertexShader* mVertShader = nullptr;
	FragmentShader* mFragShader = nullptr;
	InputLayout* mInputLayout = nullptr;
	VertexConstantBuffer* mVertCB = nullptr;
	FragmentConstantbuffer* mFragCB = nullptr;

	virtual void InitUniforms(GraphicsD11& gfx) = 0;
};