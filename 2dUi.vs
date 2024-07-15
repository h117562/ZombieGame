////////////////////////////////////////////////////////////////////////////////
// Filename: 2dUi.vs
////////////////////////////////////////////////////////////////////////////////

cbuffer MatrixBuffer: register (b0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

struct VertexInputType
{
    float4 position : POSITION;
  	float2 texcoord : TEXCOORD;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD;
};


PixelInputType vsMain(VertexInputType input)
{
    PixelInputType output;

    input.position.w = 1.0f;

    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
    output.texcoord = input.texcoord;
    
    return output;
}