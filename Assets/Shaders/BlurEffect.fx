//====================================================================================================
// Filename:	BlurEffect.fx
// Description:	Simple shader for Gaussian blur.
//====================================================================================================

Texture2D diffuseMap : register(t0);
SamplerState textureSampler : register(s0);

cbuffer BlurBuffer : register(b0)
{
    float4 blurDir;
}

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
    const float weight[] =
    {
        0.2270270270f, 0.1945945946f, 0.1216216216f,
		0.0540540541f, 0.0162162162f
    };
    float2 uv = input.texCoord;

    float4 textureColor = float4(0, 0, 0, 0);

    float deltaX = blurDir.x / blurDir.z;
    float deltaY = blurDir.y / blurDir.w;
    float2 deltaDir = float2(deltaX, deltaY);

    textureColor += diffuseMap.Sample(textureSampler, uv) * weight[0];
    textureColor += diffuseMap.Sample(textureSampler, uv + float2(deltaDir * 1)) * weight[1];
    textureColor += diffuseMap.Sample(textureSampler, uv + float2(deltaDir * 2)) * weight[2];
    textureColor += diffuseMap.Sample(textureSampler, uv + float2(deltaDir * 3)) * weight[3];
    textureColor += diffuseMap.Sample(textureSampler, uv + float2(deltaDir * 4)) * weight[4];
    textureColor += diffuseMap.Sample(textureSampler, uv - float2(deltaDir * 1)) * weight[1];
    textureColor += diffuseMap.Sample(textureSampler, uv - float2(deltaDir * 2)) * weight[2];
    textureColor += diffuseMap.Sample(textureSampler, uv - float2(deltaDir * 3)) * weight[3];
    textureColor += diffuseMap.Sample(textureSampler, uv - float2(deltaDir * 4)) * weight[4];

    textureColor.a = 1.0f;

    return textureColor;

}
