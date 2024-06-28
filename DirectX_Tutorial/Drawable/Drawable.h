#pragma once
#include "../Graphics.h"
#include <DirectXMath.h>
#include "../Macros/GraphicsThrowMacros.h"

namespace wrl = Microsoft::WRL;
class Bindable;
class IndexBuffer;

class Drawable
{
	typedef typename graphics::Graphics Gfx;
public:
	Drawable() = default;
	Drawable(const Drawable& d) = delete;
	Drawable& operator=(const Drawable& d) = delete;

	virtual DirectX::XMMATRIX GetTransformXM() const noexcept = 0;
	virtual void Update(float dt) = 0;

	// TODO: Check IS_DEBUG macro issue. it ain't looking to be declared!!!!!!
	void Draw(Gfx& gfx) const noexcept;
	void AddBind(std::unique_ptr<Bindable> bind) noexcept;
	void AddIndexBuffer(std::unique_ptr<IndexBuffer> ib)noexcept;

	virtual ~Drawable() = default;
private:
	const IndexBuffer* pIbuff;
	std::vector<std::unique_ptr<Bindable>> binds;
};