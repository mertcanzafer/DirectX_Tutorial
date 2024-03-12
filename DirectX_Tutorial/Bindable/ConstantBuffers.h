#pragma once
#include "Bindable.h"
namespace wrl = Microsoft::WRL;

/* *****************************************************************************************
   Constant Buffers are advanced topic in Graphics Programming
   They can be very efficient but painful when using it
   So, It's recommended that checking nvidia's article
   about how to use constant buffers in d3d11 / similar versions
   link: https://developer.nvidia.com/content/constant-buffers-without-constant-pain-0
********************************************************************************************/

template<class C>
class ConstantBuffers: public Bindable
{
	typedef typename graphics::Graphics Gfx;
public:
	// Every Constant buffer should be updated during each render time
	void update(Gfx& gfx, const C& constDatas);
	// Overloaded Constructor
	ConstantBuffers(Gfx& gfx,const C& constDatas);
	ConstantBuffers(Gfx& gfx);

protected:
	wrl::ComPtr<ID3D11Buffer> pConstantBuffer
};

template<class C>
class VSConstantBuffers : public ConstantBuffers<C>
{
	typedef typename graphics::Graphics Gfx;
	// Get the protected attrbitues explicitly from templated base class!!
	using Bindable::GetDeviceContext;
	using ConstantBuffers<C>::pConstantBuffer;
public:
	using ConstantBuffers<C>::ConstantBuffers;
	inline void Bind(Gfx& gfx)noexcept override
	{
		GFX_THROW_INFO(	GetDeviceContext(gfx)->VSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf()));
	}
};

template<class C>
class PSConstantBuffers : public ConstantBuffers<C>
{
	typedef typename graphics::Graphics Gfx;
	// Get the protected attrbitues explicitly from templated base class!!
	using Bindable::GetDeviceContext;
	using ConstantBuffers<C>::pConstantBuffer;
public:
	using ConstantBuffers<C>::ConstantBuffers;
	inline void Bind(Gfx& gfx)noexcept override
	{
		GFX_THROW_INFO(GetDeviceContext(gfx)->PSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf()));
	}
};