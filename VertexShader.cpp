#include "VertexShader.h"
#include "NouException.h"

VertexShader::VertexShader(GraphicsD11& gfx, const std::wstring& path) 
{

	HRESULT res;

	res = D3DReadFileToBlob(path.c_str(), &pBlob);
	CHECK_HR_EXCEPT(res);

	res = GetDevice(gfx)->CreateVertexShader(
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		nullptr,
		&pVertexShader
	);
	CHECK_HR_EXCEPT(res);
}

void VertexShader::Bind(GraphicsD11& gfx) noexcept
{
	GetContext(gfx)->VSSetShader(pVertexShader.Get(), nullptr, 0u);
}

ID3DBlob* VertexShader::GetBytecode() const noexcept
{
	return pBlob.Get();
}