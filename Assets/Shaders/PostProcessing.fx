//====================================================================================================
// Filename:	PostProcessing.fx
// Description:	Simple shader for post processing.
//====================================================================================================

Texture2D diffuseMap : register(t0);
SamplerState textureSampler : register(s0);

struct VS_INPUT
{
	float4 position	: POSITION;
	float2 texCoord	: TEXCOORD;
};

struct VS_OUTPUT
{
	float4 position	: SV_POSITION;
	float2 texCoord	: TEXCOORD;
};

VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
    output.position = input.position;
	output.texCoord = input.texCoord;
	return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
	// pixel effect
	float2 uv = input.texCoord;
	float u = (int) (uv.x * 100) / 100.0f;
	float v = (int) (uv.y * 60) / 60.0f;
	float4 color = diffuseMap.Sample(textureSampler, float2(u,v)); 
	return color;

	// Grey scale
	//float4 color = diffuseMap.Sample(textureSampler, input.texCoord); 
	//float4 average = (color.r + color.g + color.b) / 3.0f; 
	//return average;

	// Negative
	// float4 color = diffuseMap.Sample(textureSampler, input.texCoord);
	// float4 output = float4(1, 1, 1, 1);
	// output -= color;
	// return output;
	
	//return diffuseMap.Sample(textureSampler, input.texCoord);
}
