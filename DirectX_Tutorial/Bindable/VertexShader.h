#pragma once
#include "Bindable.h"
namespace wrl = Microsoft::WRL;

class VertexShader: public Bindable
{
	typedef typename graphics::Graphics Gfx;
public:
	VertexShader(Gfx& gfx, const std::wstring& VSfileLoc);
	void Bind(Gfx& gfx)noexcept override;
	ID3DBlob* GetByteCode()const noexcept;
protected:
	wrl::ComPtr<ID3DBlob>pBytecodeBlob;
	wrl::ComPtr<ID3D11VertexShader>pVertexShader;
};