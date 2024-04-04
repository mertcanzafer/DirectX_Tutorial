#pragma once
#include "Bindable.h"
#include <vector>
namespace wrl = Microsoft::WRL;

class VertexBuffer: public Bindable
{
	typedef typename graphics::Graphics Gfx;
public:
	template <class C>
	VertexBuffer(Gfx& gfx, const std::vector<C>& vertices):
		stride(sizeof(C))
	{
		INFOMAN(gfx);

		D3D11_BUFFER_DESC bd = {};
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0u;
		bd.MiscFlags = 0u;
		bd.ByteWidth = UINT(sizeof(C) * vertices.size()); // size of vertices in byte
		bd.StructureByteStride = sizeof(V);

		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = vertices;

		GFX_THROW_INFO(	GetDevice(gfx)->CreateBuffer(&bd, &sd, &pVertexBuffer) );
	}
		
	inline void Bind(Gfx& gfx)noexcept override
	{
		const UINT offset = 0u;
		GetDeviceContext(gfx)->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);
	}

protected:
	unsigned int stride;
	wrl::ComPtr<ID3D11Buffer>pVertexBuffer;
};
