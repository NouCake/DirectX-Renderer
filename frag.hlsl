struct v2f
{
	float4 pos : SV_POSITION;
	float4 col : Color;
};

float4 main(v2f input) : SV_TARGET
{
	return input.col;
}