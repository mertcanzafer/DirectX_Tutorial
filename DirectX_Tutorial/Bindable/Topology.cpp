#include "Topology.h"

Topologhy::Topologhy(Gfx& gfx, D3D11_PRIMITIVE_TOPOLOGY type)
	:type{type}{}

void Topologhy::Bind(Gfx& gfx) noexcept
{
	GetDeviceContext(gfx)->IASetPrimitiveTopology(type);
}
