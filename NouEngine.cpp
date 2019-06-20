#include "NouEngine.h"

NouEngine::NouEngine()
	:
	window(640, 480, "NouEnginge")
{

	
}

int NouEngine::Run()
{

	while (true)
	{
		
		if (NouWindow::ProcessMessage())
		{
			return NouWindow::LastQuitCode;
		}

		ExecuteFrame();
	}

	return -1;
}

void NouEngine::ExecuteFrame()
{

	window.Gfx().OnFrameEnd();
}