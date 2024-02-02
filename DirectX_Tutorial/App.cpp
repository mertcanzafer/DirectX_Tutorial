#include "App.h"

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
	window->Gfx().ClearBuffer(1.0f, c,0.0f);
	window->Gfx().EndFrame();
}

App::~App()
{
	delete window;
}
