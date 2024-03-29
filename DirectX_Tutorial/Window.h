#pragma once
#include "FixedWin.h"
#include <string>
#include <cstring>
#include <sstream>

// First add Window Exception
#include "Exception_Handler.h"
#include "resource.h"

#include "Keyboard.h"
#include "Mouse.h"
#include <optional>

#include "Graphics.h"

class Window
{
public:
	class Exception :public Exception_Handler
	{
	public:
		Exception(int line,const char* file,HRESULT hr)noexcept;
		const char* what()const noexcept override;
		const char* m_getType()const noexcept override;

		static std::string m_TranslateErrorCode(HRESULT hr)noexcept;
		HRESULT m_getErrorCode()const noexcept;
		std::string m_getErrorString()const noexcept;
	private:
		HRESULT hr;
	};

	class NoGFXException : public Exception_Handler
	{
	public:
		NoGFXException(int line, const char* file)noexcept;
		const char* m_getType()const noexcept override;
	};

private:
	// singleton manages registration/cleanup of window class
	class WindowClass 
	{
	  public:
		  static const char* getName() noexcept;
		  static HINSTANCE getInstance() noexcept;	
	  private:
		  WindowClass()noexcept;
		  ~WindowClass();

		  WindowClass(const WindowClass& copy) = delete;
		  WindowClass& operator=(const WindowClass& rhs) = delete;

		  static constexpr const char* wndClassName = "Direct3D Engine Window";
		  static WindowClass wndClass;
		  HINSTANCE hInstance;
	};

	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg,
		WPARAM wParam, LPARAM lParam)noexcept;
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg,
		WPARAM wParam, LPARAM lParam)noexcept;
	LRESULT HandleMsg(HWND hWnd, UINT msg,
		WPARAM wParam, LPARAM lParam)noexcept;

	UINT32 width;
	UINT32 height;
	HWND hWnd;
	static int WindowObjectCount;
	graphics::Graphics* pGfx;

private:
	void DestroyGfx();
public:
	Window(UINT32 width, UINT32 height,const char* name);
	~Window();

	Window(const Window& copy) = delete;
	Window& operator=(const Window& rhs) = delete;

	void m_SetTitle(const char* title)noexcept;
	static std::optional<int> ProcessMessages();
	graphics::Graphics& Gfx()const;
public:
	Keyboard kbd;
	Mouse mouse;
};

// Error exception helper macro

#define CHWND_EXCEPT(hr) Window::Exception( __LINE__,__FILE__,hr)
#define CHWND_LAST_EXCEPT() Window::Exception(__LINE__,__FILE__,GetLastError());
#define CHWND_NO_GFX_EXCEPT() Window::NoGFXException(__LINE__,__FILE__)