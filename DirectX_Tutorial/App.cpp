#include "App.h"

// This program is forced to run on NVIDIA Graphics Card!!!!!!!
extern "C" {
	__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}

App::App()
	:window{nullptr}
{
	window = new Window(980, 720, "<Direct3D> Graphics Program");
}

int 
App::Go()
{
	while (true)
	{
		// process all msgs pending, but do not block the app
		if (const auto ecode = Window::ProcessMessages())
		{
			// if return optional has a value, mean we're exiting the code
			return *ecode;
		}
		DoFrame();
	}
}

void App::
DoFrame()
{
	const float c = sin(timer.Peek()) / 2.0f + 0.5f;
	window->Gfx().ClearBuffer(0.0f, 0.0f,0.0f);
	window->Gfx().DrawTestTriangle();
	window->Gfx().EndFrame();
}

App::~App()
{
	delete window;
}
