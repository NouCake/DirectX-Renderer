#pragma once

#include "settings.h"
#include "ImGUI/imgui.h"

#include "Transform.h"

#include <DirectXMath.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace dx = DirectX;

class Camera
{
public:
	Camera(const unsigned int width, const unsigned int height);
	~Camera() = default;

#ifdef USE_IMGUI
	void SpawnImGuiControl()
	{
		ImGui::Begin("Camera");
		ImGui::SliderFloat3("Position", &mPos.m128_f32[0], -15, 15);

		static float rot[3] = { 0.0f, 0.0f, 0.0f };
		if (ImGui::SliderFloat3("Rotation", &rot[0], -180, 180))
		{

			mRotQuat = dx::XMQuaternionMultiply(dx::XMQuaternionMultiply(dx::XMQuaternionRotationAxis({ 1.0f, 0.0f, 0.0f }, -rot[0] * dx::XM_PI / 180.0f),
				dx::XMQuaternionRotationAxis({ 0.0f, 1.0f, 0.0f }, -rot[1] * dx::XM_PI / 180.0f)),
				dx::XMQuaternionRotationAxis({ 0.0f, 0.0f, 1.0f },- rot[2] * dx::XM_PI / 180.0f));
		}

		ImGui::SliderFloat("FOV", &mAngle, 0.1f, 3*dx::XM_PI/ 4.0f);
		ImGui::End();
	}
#endif

	dx::XMMATRIX GetMatrix()
	{

		return dx::XMMatrixTranspose(
			dx::XMMatrixTranslation(-mPos.m128_f32[0], -mPos.m128_f32[1], -mPos.m128_f32[2]) *
			dx::XMMatrixRotationQuaternion(mRotQuat) *
			dx::XMMatrixPerspectiveFovLH(mAngle, mAspectRation, mNear, mFar)
		);
	}
	dx::XMVECTOR mPos = { 0.0f, 0.0f, 0.0f };

private:
	float mNear = 0.1f;
	float mFar = 100.0f;
	float mAngle = 100.0f * dx::XM_PI / 180.0f;
	float width;
	float height;
	float mAspectRation = width / height;

	dx::XMVECTOR mRotQuat = { };

};