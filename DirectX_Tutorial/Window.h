#pragma once
#include "FixedWin.h"
#include <string>
#include <cstring>
#include <sstream>
class Window
{
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
public:
	Window(UINT32 width, UINT32 height,const char* name)noexcept;
	~Window();

	Window(const Window& copy) = delete;
	Window& operator=(const Window& rhs) = delete;
};

