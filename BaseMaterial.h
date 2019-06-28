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
		struct Normal
		{
			float x;
			float y;
			float z;
		} normal;
	};
	struct VertexUniforms
	{
		DirectX::XMMATRIX ObjectToWorld;
		DirectX::XMMATRIX WorldToView;
	};

	BaseMaterial(GraphicsD11& gfx);
	~BaseMaterial() = default;
	void Draw(GraphicsD11& gfx, Renderable& rend) override;
	void Begin(GraphicsD11& gfx, Camera& cam) override;
	void UpdateUniforms(GraphicsD11& gfx, Renderable& rend);

private:
	VertexUniforms mCurUniforms;
};

