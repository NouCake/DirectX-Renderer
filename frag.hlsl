struct v2f
{
	float4 pos : SV_POSITION;
	float4 col : Color;
	float2 uv : UV;
};

Texture2D tex;

SamplerState splr;

float4 main(v2f input) : SV_TARGET
{
	float4 col =  tex.Sample(splr, input.uv);
	return col;
}