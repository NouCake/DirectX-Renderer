#pragma once

#include "Bindable.h"

class Texture : public Bindable
{
public:
	Texture(GraphicsD11& gfx);
	void Bind(GraphicsD11& gfx) noexcept override;
private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureView;
};