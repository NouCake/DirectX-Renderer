#include "Topology.h"

Topology::Topology(GraphicsD11& gfx, D3D11_PRIMITIVE_TOPOLOGY type)
	:
	type(type)
{

}

void Topology::Bind(GraphicsD11& gfx) noexcept
{
	GetContext(gfx)->IASetPrimitiveTopology(type);
}