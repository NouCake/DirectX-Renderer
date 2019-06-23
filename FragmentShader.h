#pragma once

#include "Bindable.h"

#include <string>

class FragmentShader : public Bindable
{
public:
	FragmentShader(GraphicsD11& gfx, const std::wstring& path);
	void Bind(GraphicsD11& gfx) noexcept override;
protected:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
};