#pragma once

#include "settings.h"
#include "ImGUI/imgui.h"

#include <DirectXMath.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

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
		if (ImGui::SliderFloat3("Position", pos.m128_f32, -10, 10))
		{
			dirty = true;
		}
		if (ImGui::SliderFloat3("Rotation", rot.m128_f32, -180, 180))
		{
			dirty = true;
		}
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
			dx::XMMatrixTranslationFromVector(dx::XMVectorScale(pos, -1)) *
			dx::XMMatrixRotationRollPitchYawFromVector(dx::XMVectorScale(rot, -1 * 3.1415f / 180.0f)) *
			dx::XMMatrixPerspectiveFovLH(mAngle, mAspectRation, mNear, mFar)
		);
	}
};