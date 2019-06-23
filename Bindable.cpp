#include "Bindable.h"

ID3D11DeviceContext* Bindable::GetContext(GraphicsD11& gfx) noexcept
{
	return gfx.pContext.Get();
}

ID3D11Device* Bindable::GetDevice(GraphicsD11& gfx) noexcept
{
	return gfx.pDevice.Get();
}