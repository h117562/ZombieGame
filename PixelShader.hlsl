Texture2D diffTexture;
SamplerState SampleType;

struct Pin
{
	float4 pos : SV_POSITION;
	float2 texcoord : TEXCOORD;
};

float4 psMain(Pin input) : SV_TARGET
{
	float4 textureColor = diffTexture.Sample(SampleType, input.texcoord);

	return textureColor;
}