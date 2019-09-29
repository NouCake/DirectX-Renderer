#include "Texture.h"
#include "NouException.h"

#define STB_IMAGE_IMPLEMENTATION
#include "STBImage/stb_image.h"

Texture::Texture(GraphicsD11& gfx, std::string path)
{
	int width, height, channels;
	this->path = path;

	unsigned char* imgdata = stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb_alpha);
	if (imgdata == nullptr)
	{
		throw NouException::BaseException(__LINE__, __FILE__, "Could not find file : " + path);
		width = 1;
		height = 1;
		unsigned char data[] = { 255, 0, 255, 255 };
		imgdata = data;
	}

	if (channels < 3)
	{
		int none = 3;
	}

	D3D11_TEXTURE2D_DESC td = {};
	td.Width = width;
	td.Height = height;
	td.MipLevels = 0;
	td.ArraySize = 1;
	td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	td.SampleDesc.Count = 1;
	td.SampleDesc.Quality = 0;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	td.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = imgdata; //TODO
	sd.SysMemPitch = width * sizeof(unsigned char) * 4; //TODO

	Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture;

	D3D11_SHADER_RESOURCE_VIEW_DESC vd = {};
	vd.Format = td.Format;
	vd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	vd.Texture2D.MostDetailedMip = 0;
	vd.Texture2D.MipLevels = -1;




	HRESULT res = GetDevice(gfx)->CreateTexture2D(&td, nullptr, &pTexture);
	CHECK_HR_EXCEPT();
	res = GetDevice(gfx)->CreateShaderResourceView(pTexture.Get(), &vd, &pTextureView);
	CHECK_HR_EXCEPT();
	GetContext(gfx)->UpdateSubresource(pTexture.Get(), 0, 0, sd.pSysMem, sd.SysMemPitch, 0);
	GetContext(gfx)->GenerateMips(pTextureView.Get());

	stbi_image_free(imgdata);
}

void Texture::Bind(GraphicsD11& gfx) noexcept
{
	GetContext(gfx)->PSSetShaderResources(mSlot, 1u, pTextureView.GetAddressOf());
}

void Texture::SetSlot(UINT slot)
{
	mSlot = slot;
}