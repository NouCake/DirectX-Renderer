struct v2f
{
	float4 pos : SV_POSITION;
	float2 uv : UV;
	float3 normal : Normal;
	float3 camDir: CameraDirection;
	float3 worldPos : WorldPos;
};

Texture2D tex : register(t0);
Texture2D texSpecular : register(t1);

SamplerState smplr;

cbuffer Uniforms
{
	uint TextureMode;
	float LightIntensity;
	float2 LightPosition;
};

float4 main(v2f input) : SV_TARGET
{
	float2 uv = input.uv;
	if (uv.x < 0 && uv.y < 0) return float4(1.0f, 0.0f, 0.0f, 1.0f);
	return float4(input.uv.x, input.uv.y, 0.0f, 1.0f);
}