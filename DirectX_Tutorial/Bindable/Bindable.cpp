#include "Bindable.h"

ID3D11DeviceContext* Bindable::GetDeviceContext(Gfx& gfx) noexcept
{
	return gfx.pImmediateContext.Get();
}

ID3D11Device* Bindable::GetDevice(Gfx& gfx) noexcept
{
	return gfx.pDevice.Get();
}

DxgiInfoManager& Bindable::GetInfoManager(Gfx& gfx) noexcept
{
#ifndef  NDEBUG
	return gfx.infoManager;
#else
	std::logic_error("Tried to access gfx.infoManager in Release config!!!\n");
#endif //  NDEBUG
}
