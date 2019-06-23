#pragma once

#include "Bindable.h"

class IndexBuffer : public Bindable
{
public:
	IndexBuffer(GraphicsD11& gfx, UINT count, void* data);
	void Bind(GraphicsD11& gfx) noexcept override;

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer;
};