#pragma once

#include "GraphicsD11.h"

class Bindable
{
public:
	virtual void Bind(GraphicsD11& gfx) noexcept = 0; 
	virtual ~Bindable() = default;

protected:
	static ID3D11Device* GetDevice(GraphicsD11& gfx) noexcept;
	static ID3D11DeviceContext* GetContext(GraphicsD11& gfx) noexcept;
};