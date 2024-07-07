#pragma once
#include "ConstantBuffers.h"
#include "../Drawable/Drawable.h"
#include <DirectXMath.h>

class TransformCbuf: public Bindable
{
	typedef typename graphics::Graphics Gfx;
public:
	TransformCbuf(Gfx& gfx, const Drawable& parent);
	void Bind(Gfx& gfx)noexcept override;
private:
	VSConstantBuffers<DirectX::XMMATRIX> vcbuf;
	const Drawable& parent;
};