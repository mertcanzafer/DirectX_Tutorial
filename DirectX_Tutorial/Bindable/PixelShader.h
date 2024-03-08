#pragma once
#include "Bindable.h"
namespace wrl = Microsoft::WRL;

class PixelShader : public Bindable
{
	typedef typename graphics::Graphics Gfx;
public:
	PixelShader(Gfx& gfx, const std::wstring& PSfileLoc);
	void Bind(Gfx& gfx)noexcept override;
protected:
	wrl::ComPtr<ID3D11PixelShader> pPixelShader;
};