#pragma once
#include "Bindable.h"
namespace wrl = Microsoft::WRL;
class InputLayout : public Bindable
{
public:
	typedef typename graphics::Graphics Gfx;
	InputLayout(Gfx& gfx,
		const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout,
		ID3DBlob* pVertexShaderBytecode
	);
	void Bind(Gfx& gfx)noexcept override;

protected:
	wrl::ComPtr<ID3D11InputLayout> pInputLayout;
};