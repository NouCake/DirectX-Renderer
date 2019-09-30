#include "NouEngine.h"


#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#ifdef USE_IMGUI
	#include "ImGUI/imgui.h"
	#include "ImGUI/imgui_impl_win32.h"
	#include "ImGUI/imgui_impl_dx11.h"
#endif

NouEngine::NouEngine()
	:
	window(1280, 720, "NouEnginge") 
{

}


	


int NouEngine::Run() {

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

void NouEngine::ExecuteFrame() {

#ifdef USE_IMGUI
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
#endif

	float t = timer.Mark();
	GraphicsD11* g = &window.Gfx();
	g->ClearBuffer(0.2f, 0.2f, 1, 1);

#ifdef USE_IMGUI
	static float speedMultiplier = 0.0f;
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::SliderFloat("speed", &speedMultiplier, 0.0f, 2.0f);
	t *= speedMultiplier;
#endif

	static Camera* cam = new Camera(window.GetWidth(), window.GetHeight());
	cam->SpawnImGuiControl();

	g->OnFrameEnd();

}