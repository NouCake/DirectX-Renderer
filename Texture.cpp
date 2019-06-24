#include "Texture.h"
#include "NouException.h"

#define STB_IMAGE_IMPLEMENTATION
#include "CImg/stb_image.h"

Texture::Texture(GraphicsD11& gfx)
{
	int width, height, channels;

	unsigned char* imgdata = stbi_load("cube.png", &width, &height, &channels, STBI_rgb_alpha);

	D3D11_TEXTURE2D_DESC td = {};
	td.Width = width;
	td.Height = height;
	td.MipLevels = 1;
	td.ArraySize = 1;
	td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	td.SampleDesc.Count = 1;
	td.SampleDesc.Quality = 0;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	td.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = imgdata; //TODO
	sd.SysMemPitch = width * sizeof(unsigned char) * 4; //TODO

	Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture;
	HRESULT res = GetDevice(gfx)->CreateTexture2D(
		&td, &sd, &pTexture
	);
	CHECK_HR_EXCEPT();

	stbi_image_free(imgdata);

	D3D11_SHADER_RESOURCE_VIEW_DESC vd = {};
	vd.Format = td.Format;
	vd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	vd.Texture2D.MostDetailedMip = 0;
	vd.Texture2D.MipLevels = 1;
	res = GetDevice(gfx)->CreateShaderResourceView(pTexture.Get(), &vd, &pTextureView);
	CHECK_HR_EXCEPT();


	Microsoft::WRL::ComPtr<ID3D11SamplerState> pSampler;
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	GetDevice(gfx)->CreateSamplerState(&samplerDesc, &pSampler);
	GetContext(gfx)->PSSetSamplers(0, 1, pSampler.GetAddressOf());
}

void Texture::Bind(GraphicsD11& gfx) noexcept
{
	GetContext(gfx)->PSSetShaderResources(0u, 1u, pTextureView.GetAddressOf());
}