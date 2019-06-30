#include "Camera.h"

Camera::Camera(const unsigned int width, const unsigned int height)
	:
	width(width),
	height(height)
{
	mTransform.SetPosition({ 0.0f, -2.0f, 0.0f, 0.0f });
	mTransform.SetRotation(dx::XMQuaternionRotationAxis({ 0.0f, 1.0f, 0.0f }, dx::XM_PI / 2.0f));
}

dx::XMMATRIX Camera::GetMatrix()
{
	CalculateMatrix();
	return matrix;
}

void Camera::LookAt(float x, float y, float z)
{

}