#pragma once

#include "Bindable.h"

#include <vector>

class InputLayout : public Bindable
{
public:
	InputLayout(GraphicsD11& gfx, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, ID3DBlob* pVertexShaderBytecode);
	void Bind(GraphicsD11& gfx) noexcept override;

protected:
	Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
};