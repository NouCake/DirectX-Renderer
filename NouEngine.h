#pragma once

#include "NouWindow.h"

class NouEngine
{
public:
	NouEngine();
	int Run();

private:
	NouWindow window;
	void ExecuteFrame();

};