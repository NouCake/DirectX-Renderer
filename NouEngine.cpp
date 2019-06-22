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
	GraphicsD11* g = &window.Gfx();
	const float t = timer.Mark();
	static float r;

	const GraphicsD11::ConstantBuffer cb = {
		1,	0,	0,	r,
		0,	1,	0,	0,
		0,	0,	1,	0,
		0,	0,	0,	1
	};

	g->UpdateGeometry(cb);
	r += t;
	if (r > 1) r = 0;
	g->ClearBuffer(r, 1, 0, 1);
	g->DrawTriangle();

	g->OnFrameEnd();
	
}