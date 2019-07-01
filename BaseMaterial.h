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
		DirectX::XMVECTOR CamPos;
		UINT16 TextureMode;
	};
	struct FragmentUniforms
	{
		UINT32 TextureMode;
		float LightIntensity;
		float LightPos[2];
	};
	static const UINT32 TEXTUREMODE_DIFFUSE = 0b1;
	static const UINT32 TEXTUREMODE_AMBIENT = 0b10;
	static const UINT32 TEXTUREMODE_SPECULAR= 0b100;
	static const UINT32 TEXTUREMODE_HEIGHT = 0b1000;

	BaseMaterial(GraphicsD11& gfx);
	~BaseMaterial() = default;
	void Draw(GraphicsD11& gfx, Renderable& rend) override;
	void Begin(GraphicsD11& gfx, Camera& cam) override;
	void UpdateUniforms(GraphicsD11& gfx, Renderable& rend);

	void SetTextureMode(UINT32 TextureMode);

private:
	VertexUniforms mCurVertexUniforms;
	FragmentUniforms mCurFragUniforms;
	bool* gTM = nullptr;
};

