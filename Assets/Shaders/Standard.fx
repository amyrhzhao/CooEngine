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
Texture2D depthMap : register(t5);

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
	float displacementWeight;
	float useNormal;
	float aoMapWeight;
	float useShadow;
	float depthBias;
}

cbuffer ShadowBuffer : register(b4)
{
	matrix WVPLight;
}

struct VSInput
{
	float3 position : POSITION;
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
	float4 positionNDC : TEXCOORD3;
};

VSOutput VS(VSInput input)
{
	VSOutput output = (VSOutput) 0;

	float displacement = displacementMap.SampleLevel(textureSampler, input.texCoord, 0).x;
	float3 position = input.position + (input.normal * displacement * displacementWeight);

	float3 normal = mul(input.normal, (float3x3) World);
	
	float3 tangent = mul(input.tangent, (float3x3) World);
	float3 binormal = cross(normal, tangent);

	output.position = mul(float4(position, 1.0f), WVP);
	output.normal = normal;
	output.tangent = tangent;
	output.binormal = binormal;
	
	output.dirToLight = -LightDirection.xyz;
	output.dirToView = normalize(ViewPosition.xyz - mul(float4(position, 1.0f), World).xyz);
	output.texCoord = input.texCoord;
	
	if (useShadow)
		output.positionNDC = mul(float4(position, 1.0f), WVPLight);
	
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

	float3 normal = normalize(mul(sampledNormal, tbnw)) * useNormal + n * (1.0f - useNormal);
	float3 dirToLight = normalize(input.dirToLight);
	float3 dirToView = normalize(input.dirToView);

	float4 ambient = LightAmbient * MaterialAmbient;
	float4 ao = aoMap.Sample(textureSampler, input.texCoord) * aoMapWeight;
	ambient += ao;
	
	float d = saturate(dot(dirToLight, normal));
	float4 diffuse = d * LightDiffuse * MaterialDiffuse;

	float3 r = reflect(LightDirection.xyz, normal);
	float s = saturate(dot(dirToView, r));
	float4 specular = pow(s, MaterialPower.x) * LightSpecular * MaterialSpecular;
	
	float4 textureColor = diffuseMap.Sample(textureSampler, input.texCoord);
	float4 textureSpecular = specularMap.Sample(textureSampler, input.texCoord);
	float4 color = ((ambient + diffuse) * textureColor) + (specular * textureSpecular.x);
	
	if (useShadow)
	{
		// NDC:   +---------+        UV: +----------->
		//       /   1     /|            |(0, 0)    |
		//      +----|----+ |            |          |            
		//      |    |    | |            |          |
		//   -1 -----+----- 1            |          |
		//      |    |    |/             |----------+
		//      +----|----+              V        (1, 1)
		//          -1
		float actualDepth = 1.0f - input.positionNDC.z / input.positionNDC.w;
		float2 shadowUV = input.positionNDC.xy / input.positionNDC.w;
		shadowUV = (shadowUV + 1.0f) * 0.5f;
		shadowUV.y = 1.0f - shadowUV.y;
		if (saturate(shadowUV.x) == shadowUV.x &&
			saturate(shadowUV.y) == shadowUV.y)
		{
			float savedDepth = depthMap.Sample(textureSampler, shadowUV).r;
			if (savedDepth > actualDepth + depthBias)
				color = ambient * textureColor;
		}
	}
	
	return color;
}