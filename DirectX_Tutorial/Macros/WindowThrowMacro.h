#pragma once

// Error exception helper macro

#define CHWND_EXCEPT(hr) Window::Exception( __LINE__,__FILE__,hr)
#define CHWND_LAST_EXCEPT() Window::Exception(__LINE__,__FILE__,GetLastError());
#define CHWND_NO_GFX_EXCEPT() Window::NoGFXException(__LINE__,__FILE__)