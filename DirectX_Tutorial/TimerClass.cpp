#include "TimerClass.h"
using namespace std::chrono;

TimerClass::TimerClass()
{
	last = steady_clock::now();
}

float TimerClass::Mark()
{
	const auto old = last;
	last = steady_clock::now();
	const duration<float> deltaTime = last - old;
	return deltaTime.count();
}

float TimerClass::Peek() const
{
	return duration<float>(steady_clock::now() - last).count();
}
