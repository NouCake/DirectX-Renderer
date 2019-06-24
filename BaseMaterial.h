#pragma once

#include "Material.h"
#include "Cube.h"

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
		DirectX::XMMATRIX ObjectToWorld;
		DirectX::XMMATRIX WorldToView;
	};

	BaseMaterial(GraphicsD11& gfx);
	~BaseMaterial() = default;
	void Draw(GraphicsD11& gfx) override;
	void Begin(GraphicsD11& gfx) override;
	void UpdateUniforms(void* d);

private:
	VertexUniforms mCurUniforms;
};

