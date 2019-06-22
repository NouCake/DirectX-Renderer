#include "NouTimer.h"

using namespace std::chrono;

NouTimer::NouTimer()
{
	last = steady_clock::now();
}

float NouTimer::Mark()
{
	const auto old = last;
	last = steady_clock::now();
	const duration<float> frameTime = last - old;
	return frameTime.count();
}

float NouTimer::Peek() const
{
	return duration<float>(steady_clock::now() - last).count();
}