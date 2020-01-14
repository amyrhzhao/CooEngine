//====================================================================================================
// Filename:	Fragment.fx
// Description:	Simple shader for applying brightness and contrast.
//====================================================================================================

cbuffer FragmentBuffer : register(b0)
{
    float4 fragment;
}

Texture2D diffuseMap : register(t0);
SamplerState textureSampler : register(s0);

struct VS_INPUT
{
    float4 position : POSITION;
    float2 texCoord : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float2 texCoord : TEXCOORD;
};

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    output.position = input.position;
    output.texCoord = input.texCoord;
    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    float4 color = diffuseMap.Sample(textureSampler, input.texCoord);
    color.rgb /= color.a;

	float brightness = fragment.x;
    float contrast = fragment.y;

	// Apply contrast
    color.rgb = ((color.rgb - 0.5f) * max(contrast, 0)) + 0.5f;

	// Apply brightness
    color.rgb += brightness;

	// return final pixel color
    color.rgb *= color.a;

    return color;
}
