#include "NouEngine.h"
#include "Cube.h"

#ifdef USE_IMGUI
#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_win32.h"
#include "ImGUI/imgui_impl_dx11.h"
#endif

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

#ifdef USE_IMGUI
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
#endif

	return -1;
}

void NouEngine::ExecuteFrame()
{
#ifdef USE_IMGUI
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
#endif

	float t = timer.Mark();
	GraphicsD11* g = &window.Gfx();
	g->ClearBuffer(0.2f, 0.2f, 1, 1);

	static float speedMultiplier = 0.0f;

#ifdef USE_IMGUI
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::SliderFloat("speed", &speedMultiplier, 0.0f, 2.0f);
#endif


	t *= speedMultiplier;

	static Cube cb(*g);
	cb.Update(t);
	cb.Draw(*g);




	g->OnFrameEnd();
	
}