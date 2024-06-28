#include "InputLayout.h"

InputLayout::InputLayout
(
	Gfx& gfx, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, ID3DBlob* pVertexShaderBytecode
)
{
	INFOMAN(gfx);

	GetDevice(gfx)->CreateInputLayout
	(
		layout.data(),
		(UINT)layout.size(),
		pVertexShaderBytecode->GetBufferPointer(),
		pVertexShaderBytecode->GetBufferSize(),
		&pInputLayout
	);
}

void InputLayout::Bind(Gfx& gfx) noexcept
{
	GetDeviceContext(gfx)->IASetInputLayout(pInputLayout.Get());
}
