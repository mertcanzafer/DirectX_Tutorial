#include "Graphics.h"

#pragma comment(lib,"D3DCompiler.lib") 

namespace wrl = Microsoft::WRL;
namespace dx = DirectX;

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

		// First Step
		// Create depth stensil state
		D3D11_DEPTH_STENCIL_DESC dsDesc = {};
		dsDesc.DepthEnable = TRUE;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
		wrl::ComPtr<ID3D11DepthStencilState>pDSState;
		GFX_THROW_INFO( pDevice->CreateDepthStencilState(&dsDesc, &pDSState));

		// Bind depth state
		pImmediateContext->OMSetDepthStencilState(pDSState.Get(), 1u);
		// Second Step
		// Create depth stensil texture
		wrl::ComPtr<ID3D11Texture2D> DepthStencilTexture;
		D3D11_TEXTURE2D_DESC dtDesc = {};
		dtDesc.Width = 980u;
		dtDesc.Height = 720u;
		dtDesc.MipLevels = 1u;
		dtDesc.ArraySize = 1u;
		dtDesc.Format = DXGI_FORMAT_D32_FLOAT;
		dtDesc.SampleDesc.Count = 1u;
		dtDesc.SampleDesc.Quality = 0u;
		dtDesc.Usage = D3D11_USAGE_DEFAULT;
		dtDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		GFX_THROW_INFO(pDevice->CreateTexture2D(&dtDesc, nullptr, &DepthStencilTexture));
		
		// Create view of depth stensil texture
		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
		descDSV.Format = dtDesc.Format;
		descDSV.Texture2D.MipSlice = 0u;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

		GFX_THROW_INFO(pDevice->CreateDepthStencilView(DepthStencilTexture.Get(), &descDSV, &pDSV));
		// bind depth stensil view to OM;
		pImmediateContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), pDSV.Get());
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
		pImmediateContext->ClearDepthStencilView(pDSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0u);
	}

	void Graphics::DrawTestTriangle(float angle, float _Mpx, float _Mpz)
	{
		// Our aspect ratio calculated from our window dimensions!!!
		constexpr float aspectRatio = 1.3611f;
		constexpr UINT NUM_OF_FACES = 6u;
		
		HRESULT hr;
		struct Vertex
		{
			struct
			{
				FLOAT x;
				FLOAT y;
				FLOAT z;
			}pos;
		};
		// create vertex buffer (1 2d Triangle at the center of the screen)
		const Vertex vertices[] =
		{
			Vertex{	-1.0f, -1.0f, -1.0f,},
			Vertex{ 1.0f, -1.0f	,-1.0f,},
			Vertex{ -1.0f,	1.0f,-1.0f,	},
			Vertex{ 1.0f,	1.0f,-1.0f,	},

			Vertex{ -1.0f,	-1.0f,1.0f,},
			Vertex{	1.0f,-1.0f,1.0f,},
			Vertex{	-1.0f,1.0f,1.0f,},
			Vertex{	1.0f,1.0f,1.0f,},
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

		// create index buffer
		const unsigned short indices[] =
		{
			0,2,1,			2,3,1,
			1,3,5,			3,7,5,
			2,6,3,			3,6,7,
			4,5,7,			4,7,6,
			0,4,2,			2,4,6,
			0,1,4,			1,5,4
		};

		wrl::ComPtr<ID3D11Buffer>pIndexBuffer;
		D3D11_BUFFER_DESC ibd = {};
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.Usage = D3D11_USAGE_DEFAULT;
		ibd.CPUAccessFlags = 0u;
		ibd.MiscFlags = 0u;
		ibd.ByteWidth = sizeof(indices); // size of indices in byte
		ibd.StructureByteStride = sizeof(unsigned short);

		D3D11_SUBRESOURCE_DATA isd = {};
		isd.pSysMem = indices;
		GFX_THROW_INFO(pDevice->CreateBuffer(&ibd, &isd, &pIndexBuffer));

		// bind index buffer
		pImmediateContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);

		// Create constant buffer for transformation matrix
		struct ConstantBuffer
		{
			dx::XMMATRIX transform;
		};
		// Order: Scale, Rotate and Translate
		const ConstantBuffer cb =
		{
			dx::XMMatrixTranspose(						
				dx::XMMatrixRotationZ(angle) * 
				dx::XMMatrixRotationX(angle) *
				dx::XMMatrixTranslation(_Mpx,0.0f,_Mpz + 4.0f) *
				dx::XMMatrixPerspectiveLH(1.0f,aspectRatio,0.5f,10.0f)
			)
		};

		D3D11_BUFFER_DESC cbd{};
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.ByteWidth = sizeof(cb);
		cbd.StructureByteStride = 0u;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		D3D11_SUBRESOURCE_DATA csd{};
		csd.pSysMem = &cb;
		GFX_THROW_INFO (pDevice->CreateBuffer(&cbd, &csd, &pConstantBuffer));
		// bind constant buffer to vertex shader
		pImmediateContext->VSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());

		// Create a second constant buffer for holding color info
		struct ConstantBuffer2 
		{
			struct
			{
				FLOAT r;
				FLOAT g;
				FLOAT b;
				FLOAT a;
			}face_colors;
		};

		ConstantBuffer2 cb2[NUM_OF_FACES] =
		{
			ConstantBuffer2{1.0f,0.0f,1.0f,1.0f}, // FACE 1 = First 2 triangles
			ConstantBuffer2{1.0f,0.0f,0.0f,1.0f}, // FACE 2
			ConstantBuffer2{0.0f,1.0f,0.0f,1.0f}, // FACE 3
			ConstantBuffer2{0.0f,0.0f,1.0f,1.0f}, // FACE 4
			ConstantBuffer2{1.0f,1.0f,0.0f,1.0f}, // FACE 5
			ConstantBuffer2{0.0f,1.0f,1.0f,1.0f}, // FACE 6
		};

		D3D11_BUFFER_DESC cbd2 = {};
		cbd2.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd2.Usage = D3D11_USAGE_DYNAMIC;
		cbd2.ByteWidth = sizeof(cb2);
		cbd2.StructureByteStride = 0u;
		cbd2.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd2.MiscFlags = 0u;
		D3D11_SUBRESOURCE_DATA csd2 = {};
		csd2.pSysMem = cb2;
		GFX_THROW_INFO(	pDevice->CreateBuffer(&cbd2, &csd2, &pConstantColorBuffer));

		//bind constant buffer to pixel shader
		pImmediateContext->PSSetConstantBuffers(0u, 1u, pConstantColorBuffer.GetAddressOf());

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
			{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT ,D3D11_INPUT_PER_VERTEX_DATA,0},
		};

		GFX_THROW_INFO (pDevice->CreateInputLayout
		(
			ied, (UINT)ARRAYSIZE(ied),
			pBlob->GetBufferPointer(),
			pBlob->GetBufferSize(),
			&pInputLayout
		));

		// bind vertex layout
		pImmediateContext->IASetInputLayout(pInputLayout.Get());

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

		GFX_THROW_INFO_ONLY(pImmediateContext->DrawIndexed((UINT)ARRAYSIZE(indices),0u ,0u));
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
