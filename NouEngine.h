#pragma once

#include "settings.h"

#include "NouWindow.h"
#include "NouTimer.h"

class NouEngine
{
public:
	NouEngine();
	int Run();

private:
	NouWindow window;
	NouTimer timer;
	
	void ExecuteFrame();

};