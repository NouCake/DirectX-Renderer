struct v2f
{
	float4 pos : SV_POSITION;
	float4 col : Color;
	float2 uv : UV;
	float3 normal : Normal;
	float3 rp : RawPos;
};

struct vi
{
	float3 pos : Position;
	float4 color : Color;
	float2 uv : UV;
	float3 normal : Normal;
};

cbuffer Uniforms
{
	matrix ObjectToWorld;
	matrix WorldToView;
	float3 camPos;
};

v2f main(vi input)
{
	v2f output;
	output.pos = mul(float4(input.pos.x, input.pos.y, input.pos.z, 1), mul(ObjectToWorld, WorldToView));
	output.col = input.color;
	output.uv = input.uv;
	output.normal = input.normal;
	output.rp = length(camPos - mul(float4(input.pos.x, input.pos.y, input.pos.z, 1), ObjectToWorld));

	return output;
}