cbuffer ConstantBuffer : register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;
}

cbuffer BoneBuffer : register(b1)
{
	matrix matBones[128];
}

struct Vin {
	float4 pos : POSITION;
	float2 texcoord : TEXCOORD;
	uint4 boneid	: BONEID;
	float4 weight	: WEIGHT;
};

struct VOut {
	float4 pos : SV_POSITION;
	float2 texcoord : TEXCOORD;
};

VOut vsMain(Vin input)
{
	VOut output;

	matrix boneTransform = matBones[input.boneid[0]]* input.weight[0];
	boneTransform += matBones[input.boneid[1]]* input.weight[1];
	boneTransform += matBones[input.boneid[2]] * input.weight[2];

	output.pos = mul(input.pos, boneTransform);
	output.pos = mul(output.pos, World);
	output.pos = mul(output.pos, View);
	output.pos = mul(output.pos, Projection);

	output.texcoord = input.texcoord;

	return output;
}