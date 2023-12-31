#include "Window.h"

Window window(980, 720,"<Direct3D> Graphics Program");
Window window2(980, 720, "<Direct3D> Window");
Window window3(980, 720, "<Direct3D> Window2");

int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR pCmdLine,
	int nCmdShow)
{
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