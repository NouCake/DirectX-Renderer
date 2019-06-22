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
	const float t = timer.Mark();

	GraphicsD11* g = &window.Gfx();

	static float r;
	r += t;
	if (r > 1) r = 0;
	
	g->ClearBuffer(r, 0, 0, 1);

	g->OnFrameEnd();
}