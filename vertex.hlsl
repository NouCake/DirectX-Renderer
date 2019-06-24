struct v2f
{
	float4 pos : SV_POSITION;
	float4 col : Color;
	float2 uv : UV;
};

struct vi
{
	float3 pos : Position;
	float4 color : Color;
	float2 uv : UV;
};

cbuffer Uniforms
{
	matrix ObjectToWorld;
	matrix WorldToView;
};

v2f main(vi input)
{
	v2f output;
	output.pos = mul(float4(input.pos.x, input.pos.y, input.pos.z, 1), mul(ObjectToWorld, WorldToView));
	output.col = input.color;
	output.uv = input.uv;

	return output;
}