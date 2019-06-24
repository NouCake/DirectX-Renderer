#include "NouEngine.h"
#include "Cube.h"
#include "Camera.h"

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

	static Cube cb(*g, -1, 0, 0);
	static Cube cb2(*g, 1, 0, 0);
	static Camera cam;
	static bool single = true;
	if (single)
	{
		cam = Camera();
		
		single = false;
		BaseMaterial* mat = (BaseMaterial*)malloc(sizeof(BaseMaterial));
		BaseMaterial matty(*g);
		memcpy(mat, &matty, sizeof(BaseMaterial));
		cb.SetMaterial(mat);
		cb2.SetMaterial(mat);
	}
	cam.SpawnImGuiControl();

	static float x = -1;
	ImGui::SliderFloat("posX", &x, -3.0f, 3.0f);
	cb2.SetPosition(x, 0, 0);

	cb.Update(t);
	cb.Draw(*g);

	cb2.Update(t);
	cb2.Draw(*g);


	g->OnFrameEnd();
	
}