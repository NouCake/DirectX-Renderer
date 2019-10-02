struct v2f
{
	float4 pos : SV_POSITION;
	float2 uv : UV;
	float3 normal : Normal;
	float3 camDir: CameraDirection;
	float3 worldPos : WorldPos;
};

struct vi
{
	float3 pos : Position;
	float3 normal : Normal;
	float3 tang : Tangent;
	float3 bitang : Bitangent;
	float2 uv : UV;
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
	float4 worldPos = mul(float4(input.pos.x, input.pos.y, input.pos.z, 1), ObjectToWorld);
	output.pos = mul(worldPos, WorldToView);

	return output;
}