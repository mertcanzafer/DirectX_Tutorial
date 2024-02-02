#include "Graphics.h"


namespace graphics
{
	Graphics::Graphics(HWND hWnd)
		:pSwapChain{nullptr}, pDevice{nullptr}, pImmediateContext{nullptr},pTarget{nullptr}
	{
		DXGI_SWAP_CHAIN_DESC sd{};
		// Set the info for the buffer
		sd.BufferDesc.Width = 0;
		sd.BufferDesc.Height = 0;
		sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 0;
		sd.BufferDesc.RefreshRate.Denominator = 0;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 1;

		// SET Sampling mode aka anti - aliasing
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;

		// set the window mode
		sd.OutputWindow = hWnd;
		sd.Windowed = TRUE; 

		// Flipping effect reps
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		
		sd.Flags = 0;

		// Create device and front/back buffers, and swap chain and rendering context
		HRESULT Renderer_Device = D3D11CreateDeviceAndSwapChain
		(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			0,
			nullptr,
			0,
			D3D11_SDK_VERSION,
			&sd,
			&pSwapChain,
			&pDevice,
			nullptr,
			&pImmediateContext
		);
		// gain access to texture subresource in swap chain (back buffer)
		ID3D11Resource* pBackBuffer = nullptr;
		pSwapChain->GetBuffer(0u, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&pBackBuffer));
		
		if(pBackBuffer)
		{
			pDevice->CreateRenderTargetView
			(
				pBackBuffer,
				nullptr,
				&pTarget
			);
			pBackBuffer->Release();
		}
		else {
			MessageBox(hWnd, "Back buffer exception", "Back buffer is null", MB_OK | MB_ICONEXCLAMATION);
		}
	}

	void Graphics::EndFrame()
	{
		pSwapChain->Present(1u, 0u);
	}

	void Graphics::ClearBuffer(float red, float green, float blue) noexcept
	{
		const float color[]{ red,green,blue ,1.0f}; // RGBA format red + green + blue + Alpha Channel(1.0f)
		pImmediateContext->ClearRenderTargetView(pTarget, color);
	}

	Graphics::~Graphics()
	{
		if (pTarget)
		{
			pTarget->Release();
		}

		if (pImmediateContext)
		{
			pImmediateContext->Release();
		}

		if (pSwapChain)
		{
			pSwapChain->Release();
		}

		if (pDevice)
		{
			pDevice->Release();
		}
	}
}