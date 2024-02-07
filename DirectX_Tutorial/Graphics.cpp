#include "Graphics.h"

// graphics exception checking/throwing macros (some with dxgi infos)
#define GFX_EXCEPT_NOINFO(hr) Graphics::HrException( __LINE__,__FILE__,(hr) )
#define GFX_THROW_NOINFO(hrcall) if( FAILED( hr = (hrcall) ) ) throw Graphics::HrException( __LINE__,__FILE__,hr )

#ifndef NDEBUG
#define GFX_EXCEPT(hr) Graphics::HrException( __LINE__,__FILE__,(hr),infoManager.GetMessage() )
#define GFX_THROW_INFO(hrcall) infoManager.Set(); if( FAILED( hr = (hrcall) ) ) throw GFX_EXCEPT(hr)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException( __LINE__,__FILE__,(hr),infoManager.GetMessage() )
#else
#define GFX_EXCEPT(hr) Graphics::HrException( __LINE__,__FILE__,(hr) )
#define GFX_THROW_INFO(hrcall) GFX_THROW_NOINFO(hrcall)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException( __LINE__,__FILE__,(hr) )
#endif

namespace wrl = Microsoft::WRL;

namespace graphics
{
	Graphics::Graphics(HWND hWnd)
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

		UINT swapCreateFlags = 0u;

#ifndef NDEBUG
		swapCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif // !NDEBUG

		// Create device and front/back buffers, and swap chain and rendering context
		//
		// for checking results of d3d functions
		HRESULT hr; 
		GFX_THROW_INFO ( D3D11CreateDeviceAndSwapChain
		(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			swapCreateFlags,
			nullptr,
			0,
			D3D11_SDK_VERSION,
			&sd,
			&pSwapChain,
			&pDevice,
			nullptr,
			&pImmediateContext
		));

		// gain access to texture subresource in swap chain (back buffer)
		wrl::ComPtr< ID3D11Resource> pBackBuffer;
		GFX_THROW_INFO(pSwapChain->GetBuffer(0u, __uuidof(ID3D11Resource), &pBackBuffer));
		
		GFX_THROW_INFO(pDevice->CreateRenderTargetView
		(
			pBackBuffer.Get(),
			nullptr,
			&pTarget
		));		
	}

	void Graphics::EndFrame()
	{
		HRESULT hr = pSwapChain->Present(1u, 0u);
#ifndef NDEBUG
		infoManager.Set();
#endif // !NDEBUG

		if (FAILED(hr))
		{
			if (hr == DXGI_ERROR_DEVICE_REMOVED)
			{
				throw GFX_DEVICE_REMOVED_EXCEPT(pDevice->GetDeviceRemovedReason());
			}
			else
			{
				throw GFX_EXCEPT(hr);
			}
		}
	}

	void Graphics::ClearBuffer(float red, float green, float blue) noexcept
	{
		const float color[]{ red,green,blue ,1.0f}; // RGBA format red + green + blue + Alpha Channel(1.0f)
		pImmediateContext->ClearRenderTargetView(pTarget.Get(), color);
	}
	// Exception Stuff

	Graphics::HrException::HrException(int line, const char* file, HRESULT hr, std::vector<std::string>InfoMsgs) noexcept
		:Exception(line, file), hr{ hr } 
	{
		// join all info messages with newlines into single string
		for (const auto& m : InfoMsgs)
		{
			Info.append(m + "\n");
		}
		// Remove Final newline if exits.
		if (!Info.empty())
		{
			Info.pop_back();
		}
	}

	const char* Graphics::HrException::what() const noexcept
	{
		std::ostringstream oss;
		oss << m_getType() << std::endl
			<< "[Error code] 0x" << std::hex << std::uppercase << m_GetErrorCode()
			<< std::dec << " (" << static_cast<uint64_t>(m_GetErrorCode()) << ")" << std::endl
			<< "[Error string]" << m_getErrorString() << std::endl
			<< "[Description]" << m_GetErrorDescription() << std::endl;
			if (!Info.empty())
			{
				oss << "\n[Error Info]\n" << m_GetErrorInfo() << std::endl << std::endl;
			}
			oss<< m_getOriginString();
		
		whatBuffer = oss.str();
		return whatBuffer.c_str();
	}

	const char* Graphics::HrException::m_getType() const noexcept
	{
		return	"Graphics Exception";
	}

	HRESULT Graphics::HrException::m_GetErrorCode() const noexcept
	{
		return hr;
	}

	std::string Graphics::HrException::m_getErrorString() const noexcept
	{
		return DXGetErrorString(hr);
	}

	std::string Graphics::HrException::m_GetErrorDescription() const noexcept
	{
		char buff[512];
		DXGetErrorDescription(hr, buff, sizeof(buff));
		return buff;
	}

	std::string Graphics::HrException::m_GetErrorInfo() const noexcept
	{
		return Info;
	}

	const char* Graphics::DeviceRemovedException::m_getType() const noexcept
	{
		return "Graphics Exception [Device Removed] (DXGI_ERROR_DEVICE_REMOVED)";
	}
}
