#pragma once

#include "settings.h"
#include "ImGUI/imgui.h"

#include <DirectXMath.h>

namespace dx = DirectX;

class Camera
{
public:
	Camera();
	~Camera() = default;

	dx::XMMATRIX GetMatrix();

	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);
	void LookAt(float x, float y, float z);

#ifdef USE_IMGUI
	void SpawnImGuiControl()
	{
		ImGui::Begin("Camera");
		ImGui::InputFloat3("Position", pos.m128_f32, 1, 0);
		ImGui::InputFloat3("Rotation", rot.m128_f32, 1, 0);
		ImGui::End();
	}
#endif

private:

	float mNear = 0.1f;
	float mFar = 100.0f;
	float mAngle = 45.0f;
	float mAspectRation = 720.0f / 480.0f;

	dx::XMVECTOR pos;
	dx::XMVECTOR rot;


	bool dirty = true;
	dx::XMMATRIX matrix;
	void CalculateMatrix()
	{
		dirty = false;
		matrix = dx::XMMatrixTranspose(
			dx::XMMatrixRotationRollPitchYawFromVector(dx::XMVectorScale(rot, -1)) *
			dx::XMMatrixTranslationFromVector(dx::XMVectorScale(pos , -1)) *
			dx::XMMatrixPerspectiveFovLH(mAngle, mAspectRation, mNear, mFar)
		);
	}
};