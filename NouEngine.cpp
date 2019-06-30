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

	Assimp::Importer imp;
	std::string path = "res/sponza.obj";
	NouTimer* timer = new NouTimer();
	timer->Mark();
	const aiScene* sponzaScene = imp.ReadFile(path,
		aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_FlipUVs);
	if (sponzaScene == nullptr)
	{
		throw NouException::BaseException(__LINE__, __FILE__, "could not find File " + path);
	}
	OutputDebugString(("Loading " + path + " took: " + std::to_string(timer->Peek()) + "\n").c_str());

	TextureLoader tl = TextureLoader();
	for (int i = 0; i < sponzaScene->mNumMeshes; i++)
	{
		if(i != 258)
		meshes.push_back(Mesh(window.Gfx(), sponzaScene, i, tl));
	}

	OutputDebugString(("Loading all took: " + std::to_string(timer->Peek()) + "\n").c_str());


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

#ifdef USE_IMGUI
	static float speedMultiplier = 0.0f;
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::SliderFloat("speed", &speedMultiplier, 0.0f, 2.0f);
	t *= speedMultiplier;
#endif

	static Camera* cam = new Camera(window.GetWidth(), window.GetHeight());
	static Cube* cube = new Cube(*g);
	static Cube* cube2 = new Cube(*g);
	static BaseMaterial* mat = new BaseMaterial(*g);
	
	cam->SpawnImGuiControl();
	//cube->SpawnImGuiControl("Cube1");
	//cube2->SpawnImGuiControl("Cube2");



	mat->Begin(*g, *cam);
	//mat->Draw(*g, *cube);
	//mat->Draw(*g, *cube2);
	for (int i = 0; i < meshes.size(); i++)
	{
		mat->Draw(*g, meshes[i]);
	}

	g->OnFrameEnd();
	
}