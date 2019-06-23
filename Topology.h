#pragma once

#include"Bindable.h"

class Topology : public Bindable
{
public:
	Topology(GraphicsD11& gfx, D3D11_PRIMITIVE_TOPOLOGY type);
	void Bind(GraphicsD11& gfx) noexcept override;
protected:
	D3D11_PRIMITIVE_TOPOLOGY type;
};