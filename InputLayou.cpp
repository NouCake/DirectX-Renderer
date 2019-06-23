#include "InputLayout.h"
#include "NouException.h"

InputLayout::InputLayout(GraphicsD11& gfx, const std::vector < D3D11_INPUT_ELEMENT_DESC>& layout, ID3DBlob* pVertexShaderBytecode)
{
	HRESULT res;

	res = GetDevice(gfx)->CreateInputLayout(
		layout.data(),
		(UINT)layout.size(),
		pVertexShaderBytecode->GetBufferPointer(),
		pVertexShaderBytecode->GetBufferSize(),
		&pInputLayout
	);
	CHECK_HR_EXCEPT();
}

void InputLayout::Bind(GraphicsD11& gfx) noexcept
{
	GetContext(gfx)->IASetInputLayout(pInputLayout.Get());
}