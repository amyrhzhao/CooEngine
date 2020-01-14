//====================================================================================================
// Filename:	DoSomething.fx
// Created by:	Peter Chan
// Description: Shader that does something.
//====================================================================================================

cbuffer ConstantBuffer: register(b0)
{
	matrix wvp;
}

struct VS_INPUT
{
	float4 position : POSITION;
	float4 color : COLOR;
};

struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.position = mul(input.position, wvp);
	output.color = input.color;
	return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
	return input.color;
}
