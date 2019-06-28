#pragma once

#include "Bindable.h"
#include <string>

class Texture : public Bindable
{
public:
	Texture(GraphicsD11& gfx, std::string path);
	void Bind(GraphicsD11& gfx) noexcept override;

	std::string path;

private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureView;
};