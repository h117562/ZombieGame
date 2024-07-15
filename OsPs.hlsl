Texture2D diffTexture;
SamplerState SampleType;

struct Pin
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD;
};

float4 psMain(Pin input) : SV_TARGET
{
	float4 textureColor = diffTexture.Sample(SampleType, input.tex);

	return textureColor;
}