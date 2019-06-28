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
	Camera();
	~Camera() = default;

	dx::XMMATRIX GetMatrix();

	void LookAt(float x, float y, float z);

#ifdef USE_IMGUI
	void SpawnImGuiControl()
	{
		ImGui::Begin("Camera");
		dx::XMVECTOR vec = *transform->Position;
		if (ImGui::SliderFloat3("Position", &vec.m128_f32[0], -10, 10))
		{
			mTransform.SetPosition(vec);
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
			if (rot.m128_f32[0] != 0.0f || rot.m128_f32[1] != 0.0f || rot.m128_f32[2] != 0.0f)
				mTransform.SetRotation(dx::XMQuaternionRotationAxis(rot, angle));
		}
		ImGui::End();
	}
#endif

	const Transform* transform = &mTransform;

private:

	float mNear = 0.1f;
	float mFar = 100.0f;
	float mAngle = 45.0f;
	float mAspectRation = 720.0f / 480.0f;


	bool dirty = true;
	dx::XMMATRIX matrix;

	Transform mTransform;

	void CalculateMatrix()
	{
		dirty = false;
		mTransform.SetPosition(*mTransform.Position);
		matrix = dx::XMMatrixTranspose(
			mTransform.GetLocalTransform() * 
			dx::XMMatrixPerspectiveFovLH(mAngle, mAspectRation, mNear, mFar)
		);
	}
};