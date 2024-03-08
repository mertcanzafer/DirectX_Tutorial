#pragma once
#include "Bindable.h"

class Topologhy : public Bindable
{
	typedef typename graphics::Graphics Gfx;
public:
	Topologhy(Gfx& gfx, D3D11_PRIMITIVE_TOPOLOGY);
	void Bind(Gfx& gfx)noexcept override;

protected:
	D3D11_PRIMITIVE_TOPOLOGY type;
};
