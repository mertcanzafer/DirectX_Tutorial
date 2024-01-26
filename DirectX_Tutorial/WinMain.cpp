#include "Window.h"

int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR pCmdLine,
	int nCmdShow)
{
	try 
	{
		Window window(980, 720, "<Direct3D> Graphics Program");
		// message pump
		MSG msg;
		BOOL gReseult;
		while ((gReseult = GetMessage(&msg, nullptr, 0, 0)) > 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (gReseult == -1) return -1;
		else
		{
			return msg.wParam;
		}
	}
	catch (const Exception_Handler& e)
	{
		MessageBox(nullptr, e.what(), e.m_getType(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const std::exception& e)
	{
		MessageBox(nullptr,e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...)
	{
		MessageBox(nullptr, "No details available", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	return -1;
}