#pragma once
#include "FixedWin.h"
#include <d3d11_4.h>

#include "Exception_Handler.h"
#include "DxgiInfoManager.h"

#include "dxerr.h"
#include <wrl.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <memory.h>

#include "Macros\GraphicsThrowMacros.h"

namespace graphics
{
	class Graphics
	{
		friend class Bindable;
	public:
		class Exception :public Exception_Handler
		{
			using Exception_Handler::Exception_Handler;
		};

		class HrException :public Exception
		{
		public:
			HrException(int line, const char* file, HRESULT hr,std::vector<std::string>InfoMsgs = {})noexcept;
			const char* what()const noexcept override;
			const char* m_getType()const noexcept override;
			HRESULT m_GetErrorCode()const noexcept;
			std::string m_getErrorString()const noexcept;
			std::string m_GetErrorDescription()const noexcept;
			std::string m_GetErrorInfo()const noexcept;

		private:
			HRESULT hr;
			std::string Info;
		};

		class DeviceRemovedException : public HrException
		{
			using HrException::HrException;
		public:
			const char* m_getType()const noexcept override;
		private:
			std::string reason;
		};

		class InfoException :public Exception
		{
		public:
			InfoException(int line, const char* file, std::vector<std::string>InfoMsgs = {});
			const char* what()const noexcept override;
			const char* m_getType()const noexcept override;
			std::string m_GetErrorInfo()const noexcept;
		private:
			std::string Info;
		};

	public :
		Graphics(HWND hWnd);

		Graphics(const Graphics& copy) = delete;
		Graphics& operator= (const Graphics& copy) = delete;
		void EndFrame();
		void ClearBuffer(float red, float green, float blue)noexcept;
		void DrawTestTriangle(float angle,float _Mpx,float _Mpy);

		~Graphics() = default;
	private:
	// When building the project in debug mode (not in released mode)
#ifndef NDEBUG
		DxgiInfoManager infoManager;
#endif
		Microsoft::WRL::ComPtr<IDXGISwapChain>pSwapChain;
		Microsoft::WRL::ComPtr<ID3D11Device>pDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>pImmediateContext;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView>pTarget;
		Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantColorBuffer;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView>pDSV;
	};
}
