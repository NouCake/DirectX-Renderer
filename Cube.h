#pragma once

#include "Renderable.h"
#include "Texture.h"

#include "settings.h"
#include "ImGUI/imgui.h"

class Cube : public Renderable
{
public:
	Cube(GraphicsD11& gfx);
	~Cube() = default;

	void Bind(GraphicsD11& gfx) override;

#ifdef USE_IMGUI
	void SpawnImGuiControl(std::string name)
	{
		ImGui::Begin(name.c_str());

		ImGui::End();
	}
#endif

private:
	Texture* mTextureDiff;
};