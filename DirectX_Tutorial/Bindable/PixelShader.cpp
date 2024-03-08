#include "PixelShader.h"

PixelShader::PixelShader
(
	Gfx& gfx, const std::wstring& PSfileLoc
)
{
	INFOMAN(gfx);
	wrl::ComPtr<ID3DBlob>pBlob;
	
	GFX_THROW_INFO(D3DReadFileToBlob(PSfileLoc.c_str(), &pBlob));
	GFX_THROW_INFO(	GetDevice(gfx)->CreatePixelShader
	(
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		nullptr,
		&pPixelShader
	));
}

void PixelShader::Bind(Gfx& gfx) noexcept
{
	GetDeviceContext(gfx)->PSSetShader(pPixelShader.Get(), nullptr, 0u);
}
