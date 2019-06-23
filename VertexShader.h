#pragma once

#include "Bindable.h"

#include <string>

class VertexShader : public Bindable
{
public:
	VertexShader(GraphicsD11& gfx, const std::wstring& path);
	void Bind(GraphicsD11& gfx) noexcept override;
	ID3DBlob* GetBytecode() const noexcept;
protected:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
};