struct v2f
{
	float4 pos : SV_POSITION;
	float4 col : Color;
	float2 uv : UV;
	float3 normal : Normal;
};

Texture2D tex;

SamplerState splr;

float4 main(v2f input) : SV_TARGET
{
	return tex.Sample(splr, input.uv);
	//return float4(input.normal, 1.0f);
}