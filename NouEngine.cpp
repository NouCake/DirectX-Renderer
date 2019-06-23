#include "NouEngine.h"
#include "Cube.h"

NouEngine::NouEngine()
	:
	window(720.f, 480, "NouEnginge")
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
	g->ClearBuffer(0.2f, 0.2f, 1, 1);

	Cube cb(*g);
	cb.Draw(*g);

	g->OnFrameEnd();
	
}