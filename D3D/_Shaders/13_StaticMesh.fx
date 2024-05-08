#include "00_Global.fx"

float3 LightDirection;
Texture2D DiffuseMap;

//Reder
struct VertexInput
{
	float4 Position : Position;
	float2 Uv : Uv;
	float3 Normal : Normal;
};

struct VertexOutput
{
	float4 Position : SV_Position;
	float2 Uv : Uv;
	float3 Normal : Normal;
};

VertexOutput VS(VertexInput input)
{
	VertexOutput output;
	output.Position = WorldPosition(input.Position);
	output.Position = ViewProjection(output.Position);
	
	output.Normal = WorldNormal(input.Normal);
	
	output.Uv = input.Uv;
	
	return output;
}

float4 PS_Diffuse(VertexOutput input) : SV_Target
{
	float3 normal = normalize(input.Normal);
	float lambert = saturate(dot(normal, -LightDirection));

	float4 diffuseColor = DiffuseMap.Sample(LinearSampler, input.Uv);
	return diffuseColor * lambert;
}

float4 PS_WireFrame(VertexOutput input) : SV_Target
{
	return float4(0.5f, 0.5f, 0.5f, 1);
}


technique11 T0
{
	P_VP(P0, VS, PS_Diffuse)
	P_RS_VP(P1, FillMode_WireFrame, VS, PS_WireFrame)
}