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
		dx::XMVECTOR vec = *mTransform->Position;
		if (ImGui::SliderFloat3("Position", &vec.m128_f32[0], -10, 10))
		{
			mTransform->SetPosition(vec);
		}
		static dx::XMVECTOR rot;
		bool dirty = false;
		if (ImGui::SliderFloat3("RotationAxis", &rot.m128_f32[0], -1, 1))
		{
			dirty = true;
		}

		static float angle;
		if (ImGui::SliderFloat("Rotation", &angle, -dx::XM_PI, dx::XM_PI))
		{
			dirty = true;
		}
		if (dirty)
		{
			if(rot.m128_f32[0] != 0.0f || rot.m128_f32[1] != 0.0f || rot.m128_f32[2] != 0.0f)
			mTransform->SetRotation(dx::XMQuaternionRotationAxis(rot, angle));
		}
		ImGui::End();
	}
#endif

private:
	Texture* mTextureDiff;
};