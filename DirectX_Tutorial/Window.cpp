#include "Window.h"

#pragma comment(lib,"d3d11.lib")

Window::WindowClass Window::WindowClass::wndClass;
int Window::WindowObjectCount = 0;

Window::WindowClass::WindowClass()noexcept
	:hInstance{GetModuleHandle(nullptr)}
{
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = HandleMsgSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = getInstance();
	wc.hIcon = static_cast<HICON>(LoadImage(hInstance,MAKEINTRESOURCE(IDI_ICON1),IMAGE_ICON,32,32,0));
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = getName();
	wc.hIconSm = static_cast<HICON>(LoadImage(hInstance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 16, 16, 0));
	RegisterClassEx(&wc);
}

Window::WindowClass::~WindowClass()
{
	UnregisterClass(wndClassName, getInstance());
}

const char* Window::WindowClass:: getName()noexcept
{
	return wndClassName;
}

HINSTANCE Window::WindowClass::getInstance()noexcept
{
	return wndClass.hInstance;
}

// Window stuff

Window::Window(UINT32 width, UINT32 height, const char* name):width{width},height{height},pGfx{nullptr}
{
	// calculate window size based on desired client region size
	RECT wr;
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;

	if (AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE) == 0)
	{
		throw CHWND_LAST_EXCEPT();
	}

	// create window and get hWnd
	hWnd = CreateWindow(WindowClass::getName(), name,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
		nullptr, nullptr, WindowClass::getInstance(), this);
	// check for error
	
	if (hWnd == nullptr)
	{
		throw CHWND_LAST_EXCEPT();
	}

	// newly created window start off as hidden
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	WindowObjectCount++;

	// Create graphics object
	pGfx = new graphics::Graphics(this->hWnd);
}

void Window::DestroyGfx()
{
	delete pGfx;
}

Window::~Window()
{
	DestroyWindow(hWnd);
	DestroyGfx();
}

void Window::m_SetTitle(const char* title) noexcept
{
	if (SetWindowText(hWnd, title) == 0)
	{
		throw CHWND_LAST_EXCEPT();
	}
}

std::optional<int> Window::ProcessMessages()
{
	MSG msg{};
	// While queue has messages, remove and dispatch them(but do not block the app If there is no messages

	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		// Check for quit because peek message ain't signal this via return
		if (msg.message == WM_QUIT)
		{
			// return optional wrapping int(arg to PostQuiteMessage)
			return msg.wParam;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	// return empty optional When not quitting App
	return {};
}

graphics::Graphics& Window::Gfx() const
{
	return *pGfx;
}

LRESULT Window::HandleMsgSetup
(
	HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam
) noexcept
{
	// use create parameter passed in from CreateWindow() to store window class pointer at WinAPI side
	if (msg == WM_CREATE)
	{
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
		// set WinAPI-managed user data to store ptr to window class
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		// set message proc to normal (non-setup) handler now that setup is finished
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));

		// forward message to window class handler
		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
    }
	// if we get a message before the WM_NCCREATE message, handle with default handler
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleMsgThunk
(
	HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam
) noexcept
{
	// retrieve ptr to window class
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	// forward message to window class handler
	return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleMsg
(
	HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam
) noexcept
{
	switch (msg)
	{
	case WM_CLOSE:
	{
		if (WindowObjectCount > 1)
		{
			WindowObjectCount--;
			const int BuffSize = 50;

			char charBuff[BuffSize];
			char currWindowTitle[BuffSize];

			GetWindowText(hWnd, charBuff, 50);
			strcpy_s(currWindowTitle, charBuff);

			if (strcmp(currWindowTitle, "<Direct3D> Graphics Program") == 0)
			{
				PostQuitMessage(0);
				return 0;
			}
		}
		else 
		{
		  WindowObjectCount--;
		  PostQuitMessage(0);
		  return 0;
		}
	}
	break;
	// clear keystate when window loses focus to prevent input getting "stuck"
	case WM_KILLFOCUS:
	{
		kbd.ClearState();
	}
	break;
	case WM_KEYDOWN:
	{
	// syskey commands need to be handled to track ALT key (VK_MENU) and F10
	case WM_SYSKEYDOWN:
		if (!(lParam & 0x04000000) || kbd.AutoRepeatIsEnabled())// filter autorepeat
		{
			kbd.OnKeyPressed(static_cast<uint8_t> (wParam));
		}
	}
	break;
	case WM_KEYUP:
	{
		kbd.OnKeyReleased(static_cast<uint8_t>(wParam));
	}
	break;

	case WM_CHAR:
	{
		kbd.OnChar(static_cast<char>(wParam));
	}
	break;
	// **************************** MOUSE MESSAGES *****************************************
	case WM_MOUSEMOVE:
	{
		const POINTS p = MAKEPOINTS(lParam);
		// in client region -> log move, and log enter + capture mouse (if not previously in window)
		if ((p.x >= 0 && p.x < width) && (p.y >= 0 && p.y < height))
		{
			mouse.OnMouseMove(p.x, p.y);
			if (!mouse.IsInWindow())
			{
				SetCapture(hWnd);
				mouse.OnMouseEnter();
			}
		}
		// not in client -> log move / maintain capture if button down
		else 
		{
			if (mouse.LeftIsPressed() || mouse.RightIsPressed() || mouse.ScrollIsPressed())
			{
				mouse.OnLeftPressed(p.x, p.y);
			}
			// button up -> release capture / log event for leaving
			else 
			{
				ReleaseCapture();
				mouse.OnMouseLeave();
			}
		}
	}
	break;

	case WM_LBUTTONDOWN:
	{
		const POINTS p = MAKEPOINTS(lParam);
		mouse.OnLeftPressed(p.x, p.y);
	}
	break;

	case WM_RBUTTONDOWN:
	{
		const POINTS p = MAKEPOINTS(lParam);
		mouse.OnRightPressed(p.x, p.y);
	}
	break;
	
	case WM_MBUTTONDOWN:
	{
		const POINTS p = MAKEPOINTS(lParam);
		mouse.OnScrollPressed(p.x, p.y);
	}
	break;

	case WM_LBUTTONUP:
	{
		const POINTS p = MAKEPOINTS(lParam);
		mouse.OnLeftReleased(p.x, p.y);
	}
	break;
	
	case WM_RBUTTONUP:
	{
		const POINTS p = MAKEPOINTS(lParam);
		mouse.OnRightReleased(p.x, p.y);
	}
	break;

	case WM_MBUTTONUP:
	{
		const POINTS p = MAKEPOINTS(lParam);
		mouse.OnScrollReleased(p.x, p.y);
	}
	break;

	case WM_MOUSEWHEEL:
	{
		const POINTS p = MAKEPOINTS(lParam);
		const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
		mouse.OnWheelDelta(p.x, p.y, delta);
	}
	break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

// Window Exception stuff

Window::Exception::
Exception(int line, const char* file, HRESULT hr) noexcept:Exception_Handler(line,file),hr{hr}{}

const char* Window::Exception::
what() const noexcept
{
	std::ostringstream oss;
	oss << m_getType() << std::endl
		<< "[Error Code] " << m_getErrorCode() << std::endl
		<< "[Description] " << m_getErrorString() << std::endl
		<< m_getOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Window::Exception::m_getType() const noexcept
{
	return "Window Exception!";
}

std::string Window::Exception::m_TranslateErrorCode(HRESULT hr) noexcept
{
	LPTSTR pMsgBuff = nullptr;
	// windows will allocate memory for err string and make our pointer point to it
	const DWORD nMsgLen = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS
		| FORMAT_MESSAGE_FROM_SYSTEM,
		nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPSTR>(&pMsgBuff), 0, nullptr
	);

	if (nMsgLen == 0) return "Unidentified error code";
	
	std::string errorString{ pMsgBuff };
	LocalFree(pMsgBuff);
	return errorString;
}

HRESULT Window::Exception::m_getErrorCode() const noexcept
{
	return hr;
}

std::string Window::Exception::m_getErrorString() const noexcept
{
	return m_TranslateErrorCode(hr);
}

