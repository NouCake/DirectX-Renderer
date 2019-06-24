#include "Camera.h"

Camera::Camera()
{
	SetPosition(0, 0, -5);
	SetRotation(0, 0, 0);
}

dx::XMMATRIX Camera::GetMatrix()
{
	if (dirty) CalculateMatrix();
	return matrix;
}

void Camera::SetPosition(float x, float y, float z)
{
	pos.m128_f32[0] = x;
	pos.m128_f32[1] = y;
	pos.m128_f32[2] = z;
	dirty = true;
}

void Camera::SetRotation(float x, float y, float z)
{
	rot.m128_f32[0] = x;
	rot.m128_f32[1] = y;
	rot.m128_f32[2] = z;
	dirty = true;
}

void Camera::LookAt(float x, float y, float z)
{
	dx::XMVECTOR other = { x, y, z };
	dx::XMVECTOR newRot = dx::XMVectorATan2({ x, y, z }, { 0.0f, 0.0f, 1.0f });
	SetRotation(newRot.m128_f32[0], newRot.m128_f32[1], newRot.m128_f32[2]);
}