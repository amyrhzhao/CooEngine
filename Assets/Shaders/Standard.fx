//====================================================================================================
// Filename:	Standard.fx
// Description: Standard shader for CooEngine.
//====================================================================================================

cbuffer TransformBuffer : register(b0)
{
    matrix World;
    matrix WVP;
    float4 ViewPosition;
}

Texture2D diffuseMap : register(t0);
Texture2D specularMap : register(t1);
Texture2D displacementMap : register(t2);
Texture2D normalMap : register(t3);
Texture2D aoMap : register(t4);

SamplerState textureSampler : register(s0);

cbuffer LightBuffer : register(b1)
{
    float4 LightDirection;
    float4 LightAmbient;
    float4 LightDiffuse;
    float4 LightSpecular;
}

cbuffer MaterialBuffer : register(b2)
{
    float4 MaterialAmbient;
    float4 MaterialDiffuse;
    float4 MaterialSpecular;
    float4 MaterialPower;
}

cbuffer OptionsBuffer : register(b3)
{
    float4 options;
}

struct VSInput
{
    float4 position : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float2 texCoord : TEXCOORD;
};

struct VSOutput
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float3 binormal : BINORMAL;
    float3 tangent : TANGENT;
    float3 dirToLight : TEXCOORD0;
    float3 dirToView : TEXCOORD1;
    float2 texCoord : TEXCOORD2;
};

VSOutput VS(VSInput input)
{
    VSOutput output = (VSOutput) 0;

    float displacement = displacementMap.SampleLevel(textureSampler, input.texCoord, 0).x;
    float4 position = input.position + (float4(input.normal, 0) * displacement * options.x);

    float3 normal = mul(input.normal, (float3x3) World);
	
    float3 tangent = mul(input.tangent, (float3x3) World);
    float3 binormal = cross(normal, tangent);

    output.position = mul(position, WVP);
    output.normal = normal;
    output.tangent = tangent;
    output.binormal = binormal;
	
    output.dirToLight = -LightDirection.xyz;
    output.dirToView = normalize(ViewPosition.xyz - mul(position, World).xyz);
    output.texCoord = input.texCoord;
    return output;
}

float4 PS(VSOutput input) : SV_Target
{
    float4 normalColor = normalMap.Sample(textureSampler, input.texCoord);
    float3 sampledNormal = float3((2.0f * normalColor.xy) - 1.0f, normalColor.z);

    float3 n = normalize(input.normal);
    float3 b = normalize(input.binormal);
    float3 t = normalize(input.tangent);
    float3x3 tbnw = float3x3(t, b, n);

    float3 normal = normalize(mul(sampledNormal, tbnw)) * options.y + n * (1 - options.y);
	float3 dirToLight = normalize(input.dirToLight);
    float3 dirToView = normalize(input.dirToView);

	float4 ambient = LightAmbient * MaterialAmbient ;
	float4 ao = aoMap.Sample(textureSampler, input.texCoord) * options.z;
	
    float d = saturate(dot(dirToLight, normal));
    float4 diffuse = d * LightDiffuse * MaterialDiffuse;

    float3 r = reflect(LightDirection.xyz, normal);
    float s = saturate(dot(dirToView, r));
    float4 specular = pow(s, MaterialPower.x) * LightSpecular * MaterialSpecular;
	
    float4 textureColor = diffuseMap.Sample(textureSampler, input.texCoord);
    float4 textureSpecular = specularMap.Sample(textureSampler, input.texCoord);
	return ((ambient + diffuse + ao) * textureColor) + (specular * textureSpecular.x);
	//return (aoMap.Sample(textureSampler, input.texCoord) * options.z);
}
