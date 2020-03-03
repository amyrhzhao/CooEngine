// Description: Shader for generating a depth map.

cbuffer ConstantBuffer : register(b0)
{
	matrix wvp;
}

struct VS_INPUT
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float2 texCoord : TEXCOORD;
};

struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float4 positionNDC : TEXCOORD0;
};

VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT) 0;
	output.position = mul(float4(input.position, 1.0f), wvp);
	output.positionNDC = output.position;
	return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
	float depth = 1.0f - input.positionNDC.z / input.positionNDC.w;
	return float4(depth, 1.0f, 1.0f, 1.0f);
}
