//====================================================================================================
// Filename:	CrtEffect.fx
// Description:	Simple shader for CRT emulation.
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
    float4 color = diffuseMap.Sample(textureSampler, input.texCoord);
    
	int pp = (int) input.position.x % 3;
    float4 outcolor = float4(0, 0, 0, 1);
    if (pp == 1)
        outcolor.r = color.r;
    else if (pp == 2) 
        outcolor.g = color.g;
	else
        outcolor.b = color.b;
	
    if ((int) input.position.y % 3 == 0)
        outcolor *= float4(2,2,2,1);
    
	return outcolor;
    }
