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
		struct Tangent
		{
			float x;
			float y;
			float z;
		} tangent;
		struct Bitangent
		{
			float x;
			float y;
			float z;
		} bitangent;
	};
	struct VertexUniforms
	{
		DirectX::XMMATRIX ObjectToWorld;
		DirectX::XMMATRIX WorldToView;
		DirectX::XMVECTOR CamPos;
	};
	struct FragmentUniforms
	{
		float LightIntensity;
		float LightPos[2];
	};

	BaseMaterial(GraphicsD11& gfx);
	~BaseMaterial() = default;

	void Draw(GraphicsD11& gfx, Renderable& rend) override;
	void Begin(GraphicsD11& gfx, Camera& cam) override;
	void UpdateUniforms(GraphicsD11& gfx, Renderable& rend);

private:
	VertexUniforms mCurVertexUniforms;
	FragmentUniforms mCurFragUniforms;

};

