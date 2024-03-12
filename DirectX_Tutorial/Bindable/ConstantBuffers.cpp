#include "ConstantBuffers.h"

template<class C>
void ConstantBuffers<C>::update(Gfx& gfx, const C& constDatas)
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
	GFX_THROW_INFO(	GetDeviceContext(gfx)->Unmap(pConstantBuffer.Get(), 0u));
}

template<class C>
ConstantBuffers<C>::ConstantBuffers(Gfx& gfx, const C& constDatas)
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

template<class C>
ConstantBuffers<C>::ConstantBuffers(Gfx& gfx)
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