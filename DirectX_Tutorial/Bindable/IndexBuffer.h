#pragma once
#include "Bindable.h"
namespace wrl = Microsoft::WRL;
class IndexBuffer:public Bindable
{
	typedef typename graphics::Graphics Gfx;
public:
	IndexBuffer(Gfx& gfx,const std::vector<uint16_t>& indices);
	void Bind(Gfx& gfx)noexcept override;
	UINT getCount()const noexcept;
protected:
	UINT count;
	wrl::ComPtr<ID3D11Buffer> pIndexBuffer;
};
