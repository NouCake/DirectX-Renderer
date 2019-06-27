#include "Transform.h"

Transform::Transform()
{
	localTransform = dx::XMMatrixIdentity();

	dx::XMMatrixDecompose(&scale, &rotationQuat, &position, localTransform);
}

/*
void Transform::print()
{
	dx::XMMatrixDecompose(&scale, &rotationQuat, &position, localTransform);
	std::cout << "Position: \t(\t" << position.m128_f32[0] << ", \t" << position.m128_f32[1] << ", \t" << position.m128_f32[2] << ", \t" << position.m128_f32[3] << ")" << std::endl;
	std::cout << "Rotation: \t(\t" << rotationQuat.m128_f32[0] << ", \t" << rotationQuat.m128_f32[1] << ", \t" << rotationQuat.m128_f32[2] << ", \t" << rotationQuat.m128_f32[3] << ")" << std::endl;
	std::cout << "Scale:  \t(\t" << scale.m128_f32[0] << ", \t" << scale.m128_f32[1] << ", \t" << scale.m128_f32[2] << ", \t" << scale.m128_f32[3] << ")" << std::endl;
	std::cout << std::endl;
}
*/

void Transform::SetPosition(dx::XMVECTOR pos)
{
	position = pos;
	localTransform.r[3].m128_f32[0] = pos.m128_f32[0];
	localTransform.r[3].m128_f32[1] = pos.m128_f32[1];
	localTransform.r[3].m128_f32[2] = pos.m128_f32[2];
}

void Transform::SetRotation(dx::XMVECTOR rotationQuat)
{
	this->rotationQuat = rotationQuat;
	localTransform = dx::XMMatrixAffineTransformation(scale, {}, rotationQuat, position);
}

void Transform::RotateLocal(dx::XMVECTOR rotationQuat)
{
	SetRotation(dx::XMQuaternionMultiply(this->rotationQuat, rotationQuat));
}

void Transform::RotateAroundPoint(dx::XMVECTOR rotationQuat, dx::XMVECTOR origin)
{
	this->rotationQuat = rotationQuat;
	localTransform = dx::XMMatrixAffineTransformation(scale, dx::XMVectorSubtract(origin, position), rotationQuat, position);
}

dx::XMMATRIX Transform::GetLocalTransform()
{
	return localTransform;
}