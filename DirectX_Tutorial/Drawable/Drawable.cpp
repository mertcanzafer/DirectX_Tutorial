#include "Drawable.h"
#include"../Bindable/IndexBuffer.h"
#include <typeinfo>
#include <cassert>

void Drawable::Draw(Gfx& gfx) const noexcept
{
	for (auto& b : binds)
	{
		b->Bind(gfx);
	}

	// TODO: Implement a DrawIndexed methond inside of graphics class!!!
}

void Drawable::AddBind(std::unique_ptr<Bindable> bind) noexcept
{
	assert("*Must*  use AddIndexBuffer to bind index buffer" &&
		typeid(*bind) != typeid(IndexBuffer));
	binds.emplace_back(std::move(bind));
}

void Drawable::AddIndexBuffer(std::unique_ptr<IndexBuffer> ib) noexcept
{
	assert("Attempting to add index buffer a second time" && pIbuff == nullptr);

	pIbuff = ib.get();
	binds.emplace_back(std::move(ib));
}
