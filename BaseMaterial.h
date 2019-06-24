#pragma once

#include "Material.h"

class BaseMaterial : public Material
{
public: 
	struct VertexInput
	{
		struct Vertex
		{
			float x;
			float y;
			float z;
		} vertex;
		struct Color
		{
			UINT8 r;
			UINT8 g;
			UINT8 b;
			UINT8 a;
		} color;
		struct UV
		{
			float u;
			float v;
		} uv;
	};
	struct VertexUniforms
	{
		DirectX::XMMATRIX transform;
	};

	BaseMaterial(GraphicsD11& gfx);
	~BaseMaterial() = default;
	void Draw(GraphicsD11& gfx) override;
	void Begin(GraphicsD11& gfx) override;
	void UpdateUniforms(VertexUniforms uniform);

private:
	VertexUniforms mCurUniforms;
};

