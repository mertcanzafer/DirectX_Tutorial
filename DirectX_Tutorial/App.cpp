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
	const float t = timer.Peek();
	std::ostringstream oss;
	oss << "Time elapsed: "<<std::setprecision(2)<<t<<std::fixed<<" in seconds";
	window->m_SetTitle(oss.str().c_str());
}

App::~App()
{
	delete window;
}
