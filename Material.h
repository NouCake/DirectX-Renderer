#pragma once

#include "FragmentShader.h"
#include "VertexShader.h"
#include "InputLayout.h"
#include "ConstantBuffer.h"

class Material
{
public:
	~Material() = default;
	virtual void Draw(GraphicsD11& gfx) = 0;
	virtual void Begin(GraphicsD11& gfx) = 0;
protected:
	VertexShader* mVertShader = nullptr;
	FragmentShader* mFragShader = nullptr;
	InputLayout* mInputLayout = nullptr;
	VertexConstantBuffer* mVertCB = nullptr;
	FragmentConstantbuffer* mFragCB = nullptr;
};