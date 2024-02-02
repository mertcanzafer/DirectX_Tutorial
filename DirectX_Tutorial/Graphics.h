#pragma once
#include "FixedWin.h"
#include <d3d11_4.h>

namespace graphics
{
	class Graphics
	{
	public :
		Graphics(HWND hWnd);

		Graphics(const Graphics& copy) = delete;
		Graphics& operator= (const Graphics& copy) = delete;
		void EndFrame();
		void ClearBuffer(float red, float green, float blue)noexcept;

		~Graphics();
	private:
		IDXGISwapChain* pSwapChain;
		ID3D11Device* pDevice;
		ID3D11DeviceContext* pImmediateContext;
		ID3D11RenderTargetView* pTarget;
	};
}


