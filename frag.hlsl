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
	int none = 0;

	float3 lightDir = normalize(float3(LightPosition.x, 2.0f, LightPosition.y) - input.worldPos);
	float NdotL = dot(input.normal, lightDir);
	float3 R = 2.0f * input.normal * NdotL - lightDir;
	float RdotV = dot(R, normalize(-input.camDir));

	float4 color = tex.Sample(smplr, input.uv);
	if (color.a == 0) discard;

	float4 outColor = float4(0.0f, 0.0f, 0.0f, 0.0f);

	float ambi = 0.5f;
	float diff = max(0.0f, NdotL);
	float spec = max(0.0f, RdotV) * pow(RdotV, 4);

	if (TextureMode & 1) //DIFUSE
		outColor += LightIntensity * color * diff;

	if (TextureMode & 2) //AMBIENT
		outColor += color * ambi;

	if (TextureMode & 4) //SPECULAR
		outColor += color * LightIntensity * texSpecular.Sample(smplr, input.uv).r * spec;

	if (TextureMode & 8) //HEIGHT
		none = 0;
	
	//float s = max(0.0f, 1.0f - length(input.camDir) / 15.0f);
	//return float4(s,s,s, 1.0f);
	//return float4(input.normal * 0.5f + 0.5f, 1.0f);
	return outColor;
}