#include "00_Global.fx"

struct VertexInput
{
	float4 Position : Position;
};

struct VertexOutput
{
	float4 Position : SV_Position;
};

//-----------------------------------------------------------------------------
// Texture Mapping
//-----------------------------------------------------------------------------
struct VertexInput_Texture
{
	float4 Position : Position;
	float3 Normal : Normal;
	float2 Uv : Uv;
};

struct VertexOutput_Texture
{
	float4 Position : SV_Position;
	float3 Normal : Normal;
	float2 Uv : Uv;
};

VertexOutput_Texture VS_Texture(VertexInput_Texture input)
{
	VertexOutput_Texture output;
	output.Position = WorldPosition(input.Position);
	output.Position = ViewProjection(output.Position);

	output.Normal = WorldNormal(input.Normal);
	
	output.Uv = input.Uv;
	
	return output;
}

Texture2D BaseMap;
Texture2D LayerMap;
Texture2D AlphaMap;

float Tile = 1;
float Intensity = 1;
float3 LightDirection = float3(-1, -1, 1);

float4 PS_Texture(VertexOutput_Texture input) : SV_Target
{
	float3 normal = normalize(input.Normal);
	float lambert = saturate(dot(normal, -LightDirection));

	float4 baseColor = BaseMap.Sample(LinearSampler, input.Uv * Tile);
	float4 layerColor = LayerMap.Sample(LinearSampler, input.Uv * Tile);
	float alpha = AlphaMap.Sample(LinearSampler, input.Uv).r;
	
	float4 result = lerp(baseColor, layerColor, saturate(alpha * Intensity));
	
	return result * lambert;
}

technique11 T0
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS_Texture()));
		SetPixelShader(CompileShader(ps_5_0, PS_Texture()));
	}
	
}