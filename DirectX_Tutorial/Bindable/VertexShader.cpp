#include "VertexShader.h"
VertexShader::VertexShader
(
	Gfx& gfx, const std::wstring& VSfileLoc
)
{
	INFOMAN(gfx);
	GFX_THROW_INFO(	D3DReadFileToBlob(VSfileLoc.c_str(), &pBytecodeBlob));

	GFX_THROW_INFO(	GetDevice(gfx)->CreateVertexShader
	(
		pBytecodeBlob->GetBufferPointer(),
		pBytecodeBlob->GetBufferSize(),
		nullptr,
		&pVertexShader
	));

}

void VertexShader::Bind(Gfx& gfx) noexcept
{
	// Bind the vertex Shader
	GetDeviceContext(gfx)->VSSetShader(pVertexShader.Get(), nullptr, 0u);
}

ID3DBlob* VertexShader::GetByteCode() const noexcept
{
	return pBytecodeBlob.Get();
}
