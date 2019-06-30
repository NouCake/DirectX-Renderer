#pragma once

#include <DirectXMath.h>
#include <iostream>

namespace dx = DirectX;

class Transform
{

public:
	Transform();

	//void print();
	void SetPosition(dx::XMVECTOR pos);
	void SetRotation(dx::XMVECTOR rotationQuat);

	void Scale(float x, float y, float z);
	void RotateLocal(dx::XMVECTOR rotationQuat);
	void RotateAroundPoint(dx::XMVECTOR rotationQuat, dx::XMVECTOR origin);

	dx::XMMATRIX GetLocalTransform();

	const dx::XMVECTOR* Position = &position;
	const dx::XMVECTOR* Rotation = &rotationQuat;
	const dx::XMVECTOR* Scalar = &scale;

private:
	dx::XMMATRIX localTransform;
	
	dx::XMVECTOR scale;
	dx::XMVECTOR rotationQuat;
	dx::XMVECTOR position;
	
	const Transform* parent;

};