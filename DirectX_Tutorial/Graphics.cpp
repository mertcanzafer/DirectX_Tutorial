#include "Graphics.h"

#pragma comment(lib,"D3DCompiler.lib") 

// graphics exception checking/throwing macros (some with dxgi infos)
#define GFX_EXCEPT_NOINFO(hr) Graphics::HrException( __LINE__,__FILE__,(hr) )
#define GFX_THROW_NOINFO(hrcall) if( FAILED( hr = (hrcall) ) ) throw Graphics::HrException( __LINE__,__FILE__,hr )

#ifndef NDEBUG
#define GFX_EXCEPT(hr) Graphics::HrException( __LINE__,__FILE__,(hr),infoManager.GetMessage() )
#define GFX_THROW_INFO(hrcall) infoManager.Set(); if( FAILED( hr = (hrcall) ) ) throw GFX_EXCEPT(hr)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException( __LINE__,__FILE__,(hr),infoManager.GetMessage() )
#define GFX_THROW_INFO_ONLY(call) infoManager.Set(); (call); {auto v = infoManager.GetMessage(); if(!v.empty()){throw graphics::Graphics::InfoException( __LINE__,__FILE__,v);}} 
#else
#define GFX_EXCEPT(hr) Graphics::HrException( __LINE__,__FILE__,(hr) )
#define GFX_THROW_INFO(hrcall) GFX_THROW_NOINFO(hrcall)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException( __LINE__,__FILE__,(hr) )
#define GFX_THROW_INFO_ONLY(call) (call)
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

	void Graphics::DrawTestTriangle()
	{
		HRESULT hr;
		struct Vertex
		{
			float x;
			float y;
		};
		// create vertex buffer (1 2d Triangle at the center of the screen)
		const Vertex vertices[] =
		{
			Vertex(	0.0f,	1.0f),   //
			Vertex(	1.0f,	-1.0f), //   CW order of drawing
			Vertex(-1.0f,	-1.0f)  //
		};

		wrl::ComPtr<ID3D11Buffer>pVertexBuffer;

		D3D11_BUFFER_DESC bd = {};
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0u;
		bd.MiscFlags = 0u;
		bd.ByteWidth = sizeof(vertices); // size of vertices in byte
		bd.StructureByteStride = sizeof(Vertex);

		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = vertices;

		GFX_THROW_INFO (pDevice->CreateBuffer(&bd, &sd, &pVertexBuffer));

		// Bind the vertex pipeline
		const UINT stride = sizeof(Vertex);
		const UINT offset = 0u;

		pImmediateContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);
		
		// Create Pixel Shader
		wrl::ComPtr<ID3D11PixelShader> pPixelShader;
		wrl::ComPtr<ID3DBlob> pBlob;
		GFX_THROW_INFO(D3DReadFileToBlob(L"PixelShader.cso", &pBlob));
		GFX_THROW_INFO(pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(),
			nullptr, &pPixelShader));
		// Bind the pixel Shader
		pImmediateContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);
		
		// Create Vertex Shader
		wrl::ComPtr<ID3D11VertexShader> pVertexShader;
		GFX_THROW_INFO(D3DReadFileToBlob(L"VertexShader.cso", &pBlob));
		GFX_THROW_INFO(pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(),
			nullptr, &pVertexShader));

		// Bind the vertex Shader
		pImmediateContext->VSSetShader(pVertexShader.Get(), nullptr, 0u);

		// input (vertex) layout (2d position only)
		wrl::ComPtr<ID3D11InputLayout> pInputLayout;

		const D3D11_INPUT_ELEMENT_DESC ied[] =
		{
			{"POSITION",0,DXGI_FORMAT_R32G32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
		};

		GFX_THROW_INFO (pDevice->CreateInputLayout
		(
			ied, (UINT)std::size(ied),
			pBlob->GetBufferPointer(),
			pBlob->GetBufferSize(),
			&pInputLayout
		));

		// bind vertex layout
		pImmediateContext->IASetInputLayout(pInputLayout.Get());

		// bind render targets
		pImmediateContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), nullptr);

		// set primitive topology to triangle list(groups of 3 vertices)
		pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// Configure View Port
		D3D11_VIEWPORT vp{};
		vp.Height = 720;
		vp.Width = 980;
		vp.MinDepth = 0;
		vp.MaxDepth = 1;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		pImmediateContext->RSSetViewports(1u, &vp);

		GFX_THROW_INFO_ONLY(pImmediateContext->Draw((UINT)std::size(vertices), 0u));
	}

	// Exception Stuff

	Graphics::HrException::HrException(int line, const char* file, HRESULT hr, std::vector<std::string>InfoMsgs) noexcept
		:Exception(line, file), hr{ hr } 
	{
		// join all info messages with newlines into single string
		for (const auto& m : InfoMsgs)
		{
			Info += m;
			Info.push_back('\n');
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
	Graphics::InfoException::InfoException(int line, const char* file, std::vector<std::string> InfoMsgs)
		:Exception(line,file)
	{
		for (const auto& m : InfoMsgs)
		{
			Info += m;
			Info.push_back('\n');
		}

		if (!Info.empty())
		{
			Info.pop_back();
		}
	}
	const char* Graphics::InfoException::what() const noexcept
	{
		std::ostringstream oss;
		oss <<m_getType() << std::endl
			<< "\n[Error Info]\n" << m_GetErrorInfo() << std::endl << std::endl;
		oss << m_getOriginString();
		whatBuffer = oss.str();
		return whatBuffer.c_str();
	}
	const char* Graphics::InfoException::m_getType() const noexcept
	{
		return "Graphics Info Exception";
	}
	std::string Graphics::InfoException::m_GetErrorInfo() const noexcept
	{
		return Info;
	}
}
