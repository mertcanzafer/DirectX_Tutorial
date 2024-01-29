#pragma once
#include "Window.h"

#include "TimerClass.h"

class App
{
public:
	App();
	int Go();

	App(const App& copy) = delete;
	App& operator=(const App& copy) = delete;
private:
	void DoFrame();
private:
	Window *window;
	TimerClass timer;
public:
	~App();
};

