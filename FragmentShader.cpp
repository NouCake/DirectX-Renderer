#include "FragmentShader.h"
#include "NouException.h"

FragmentShader::FragmentShader(GraphicsD11& gfx, const std::wstring& path)
{
	HRESULT res;
	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;

	res = D3DReadFileToBlob(path.c_str(), &pBlob);
	CHECK_HR_EXCEPT(res);

	res = GetDevice(gfx)->CreatePixelShader(
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		nullptr,
		&pPixelShader
	);
	CHECK_HR_EXCEPT(res);

}

void FragmentShader::Bind(GraphicsD11& gfx) noexcept
{
	GetContext(gfx)->PSSetShader(pPixelShader.Get(), nullptr, 0u);
}