#pragma once

#include "Bindable.h"

class VertexBuffer : public Bindable
{
public:
	VertexBuffer(GraphicsD11& gfx, UINT size, UINT stride, void* data);
	void Bind(GraphicsD11& gfx) noexcept override;

private:
	UINT stride;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
};