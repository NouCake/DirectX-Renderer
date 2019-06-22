#pragma once

#include <chrono>

class NouTimer
{
public:
	NouTimer();
	float Mark();
	float Peek() const;
private:
	std::chrono::steady_clock::time_point last;
};