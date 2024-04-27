#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(Gfx& gfx, const std::vector<uint16_t>& indices)
	:count{(UINT)indices.size()} 
{
	INFOMAN(gfx);

	D3D11_BUFFER_DESC ibd;
	ZeroMemory(&ibd, sizeof(ibd));

	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0u;
	ibd.ByteWidth = (UINT)(sizeof(UINT16) * count);
	ibd.StructureByteStride = sizeof(UINT16);
	ibd.MiscFlags = 0u;

	D3D11_SUBRESOURCE_DATA isd;
	ZeroMemory(&isd, sizeof(isd));

	isd.pSysMem = indices.data();
	GFX_THROW_INFO(	GetDevice(gfx)->CreateBuffer(&ibd, &isd, &pIndexBuffer) );
}

void IndexBuffer::Bind(Gfx& gfx) noexcept
{
	UINT offset = 0u;
	GetDeviceContext(gfx)->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, offset);
}

UINT IndexBuffer::getCount() const noexcept
{
	return count;
}
