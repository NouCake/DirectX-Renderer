#include "NouEngine.h"

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
	GraphicsD11* g = &window.Gfx();
	const float t = timer.Mark();
	static float angle;
	angle = 45 * 3.1415f / 180;
	const GraphicsD11::ConstantBuffer cb = {
		DirectX::XMMatrixTranspose(
		DirectX::XMMatrixRotationZ(angle) *
		DirectX::XMMatrixScaling(480.f / 720.f, 1, 1)
		)
	};

	g->UpdateGeometry(cb);
	g->ClearBuffer(1, 1, 1, 1);
	g->DrawTriangle();

	g->OnFrameEnd();
	
}