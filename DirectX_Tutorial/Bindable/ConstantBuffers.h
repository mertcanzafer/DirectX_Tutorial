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
	void update(Gfx& gfx, const C& constDatas)
	{
		INFOMAN(gfx);

		D3D11_MAPPED_SUBRESOURCE MSR;
		ZeroMemory(&MSR, sizeof(MSR));

		GFX_THROW_INFO(GetDeviceContext(gfx)->Map
		(
			pConstantBuffer.Get(),
			0u,
			D3D11_MAP_WRITE_DISCARD,
			0u,
			&MSR
		));

		memcpy(MSR.pData, &constDatas, sizeof(constDatas));
		GFX_THROW_INFO(GetDeviceContext(gfx)->Unmap(pConstantBuffer.Get(), 0u));
	}
	// Overloaded Constructor
	ConstantBuffers(Gfx& gfx, const C& constDatas)
	{
		INFOMAN(gfx);

		D3D11_BUFFER_DESC cbd = {};
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		cbd.ByteWidth = sizeof(constDatas);
		cbd.StructureByteStride = 0u;

		D3D11_SUBRESOURCE_DATA csd = {};
		csd.pSysMem = &constDatas;

		GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&cbd, &csd, &pConstantBuffer));
	}
	ConstantBuffers(Gfx& gfx)
	{
		INFOMAN(gfx);

		D3D11_BUFFER_DESC cbd = {};
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		cbd.ByteWidth = sizeof(C);
		cbd.StructureByteStride = 0u;
		GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&cbd, nullptr, &pConstantBuffer));
	}

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