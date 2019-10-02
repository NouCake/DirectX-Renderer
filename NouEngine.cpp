#include "NouEngine.h"


#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "MeshLoader.h"

#ifdef USE_IMGUI
	#include "ImGUI/imgui.h"
	#include "ImGUI/imgui_impl_win32.h"
	#include "ImGUI/imgui_impl_dx11.h"
#endif

NouEngine::NouEngine()
	:
	window(1280, 720, "NouEnginge") 
{

	int numVerts = 4;
	Mesh::Vertex::Position vertPos[4] = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 1.0f, 0.0f},
		{1.0f, 0.0f, 0.0f},
		{1.0f, 1.0f, 0.0f}
	};
	Mesh::Vertex::Normal vertNorm[4] = {
		{0.0f, 0.0f, 1.0f},
		{0.0f, 0.0f, 1.0f},
		{0.0f, 0.0f, 1.0f},
		{0.0f, 0.0f, 1.0f}
	};
	Mesh::Vertex::Tangent vertTang[4] = {
		{1.0f, 0.0f, 0.0f},
		{1.0f, 0.0f, 0.0f},
		{1.0f, 0.0f, 0.0f},
		{1.0f, 0.0f, 0.0f}
	};
	Mesh::Vertex::Bitangent vertBitang[4] = {
		{0.0f, 1.0f, 0.0f},
		{0.0f, 1.0f, 0.0f},
		{0.0f, 1.0f, 0.0f},
		{0.0f, 1.0f, 0.0f}
	};
	Mesh::Vertex::Textcoord vertTex[4] = {
		{0.0f, 1.0f},
		{0.0f, 0.0f},
		{1.0f, 1.0f},
		{1.0f, 0.0f}
	};
	int numInds = 6;
	UINT16 indecies[6] = {
		0, 1, 3,
		3, 1, 2
	};

	GraphicsD11* g = &window.Gfx();

	MeshLoader meshl;
	Mesh* m = meshl.loadMesh("res/test.obj");

	//Mesh m(numVerts, vertPos, vertNorm, vertTang, vertBitang, vertTex, numInds, indecies);
	_bm = new BindableMesh(*g, m);
	_mat = new BaseMaterial(*g);

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

	GraphicsD11* g = &window.Gfx();
	g->ClearBuffer(0.2f, 0.2f, 1, 1);

#ifdef USE_IMGUI
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
#endif

	static Camera* cam = new Camera(window.GetWidth(), window.GetHeight());

	_mat->Begin(*g, *cam);
	_mat->Draw(*g, *_bm);

	g->OnFrameEnd();

}