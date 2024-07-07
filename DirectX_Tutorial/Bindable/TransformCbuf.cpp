#include "TransformCbuf.h"

TransformCbuf::TransformCbuf(Gfx& gfx, const Drawable& parent)
	:vcbuf{gfx},parent{parent}{}

void TransformCbuf::Bind(Gfx& gfx) noexcept
{
	// TODO: Implement GetProjection function in graphics class!!!!!!!!
	//vcbuf.update(gfx,
	//	DirectX::XMMatrixTranspose(
	//		parent.GetTransformXM() * gfx.GetProjection()
	//	)
	//);
	vcbuf.Bind(gfx);
}
