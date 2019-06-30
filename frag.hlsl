struct v2f
{
	float4 pos : SV_POSITION;
	float2 uv : UV;
	float3 normal : Normal;
	float3 camDir: CameraDirection;
};

Texture2D tex;
SamplerState smplr;



float4 main(v2f input) : SV_TARGET
{

	float3 lightDir = normalize(float3(1, 1, 1));
	float NdotL = dot(input.normal, lightDir);

	float4 color =  tex.Sample(smplr, input.uv);
	if (color.a == 0) discard;

	float ambi = 0.5f;
	float diff = max(0.0f, NdotL);
	
	float3 R = 2.0f * input.normal * NdotL - lightDir;
	float RdotV = dot(R, normalize(input.camDir));
	float spec = max(0.0f, RdotV)* pow(RdotV, 4);

	return color * (ambi + diff + spec);
	//return float4(input.normal * 0.5f + 0.5f, 1.0f);
}