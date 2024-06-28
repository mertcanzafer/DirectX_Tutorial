#pragma once
#include "..\Graphics.h"
using namespace graphics;
class Bindable
{
	typedef typename graphics::Graphics Gfx;
public:
	Bindable()noexcept = default;
	virtual void Bind(Gfx& gfx)noexcept = 0;
	virtual~Bindable()noexcept = default;
protected:
	// TODO: Check IS_DEBUG macro issue. it ain't looking to be declared!!!!!!
	static ID3D11DeviceContext* GetDeviceContext(Gfx& gfx)noexcept;
	static ID3D11Device* GetDevice(Gfx& gfx)noexcept;
	static DxgiInfoManager& GetInfoManager(Gfx& gfx)noexcept;
};