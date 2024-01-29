#pragma once
#include <chrono>

class TimerClass
{
public:
	TimerClass();

	float Mark();
	float Peek()const;
private:
	std::chrono::steady_clock::time_point last;
};

