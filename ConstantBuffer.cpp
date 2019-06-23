#include "ConstantBuffer.h"

ConstantBuffer::ConstantBuffer(GraphicsD11& gfx, UINT size, void* data)


void ConstantBuffer::Bind(GraphicsD11& gfx) noexcept
{
	GetContext(gfx)->VSSetConstantBuffers(0u, 1u, pConstBuffer.GetAddressOf());
}