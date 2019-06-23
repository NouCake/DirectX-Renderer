struct v2f
{
	float4 pos : SV_POSITION;
	float4 col : Color;
};

cbuffer CBuf
{
	matrix transform;
};

v2f main(float2 pos : Position, float4 color : Color)
{
	v2f output;
	output.pos = mul(float4(pos.x, pos.y, 0, 1), transform);
	output.pos = float4(pos, 0, 1);
	output.col = color;
	
	return output;
}