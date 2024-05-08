#include "00_Global.fx"

//Parameters
float3 LightDirection;
TextureCube SkyMap;

//Reder
struct VertexInput
{
	float4 Position : Position;
};

struct VertexOutput
{
	float4 Position : SV_Position;
	float3 Uvw : Uvw;
};

VertexOutput VS(VertexInput input)
{
	VertexOutput output;
	output.Position = WorldPosition(input.Position);
	output.Position = ViewProjection(output.Position);
	
	output.Uvw = normalize(input.Position.xyz - float3(0, 0, 0));
	
	return output;
}

float4 PS(VertexOutput input) : SV_Target
{
	return SkyMap.Sample(LinearSampler, input.Uvw);
}

technique11 T0
{
	P_VP(P0, VS, PS)
	P_RS_VP(P1, FrontCounterClockwise_True, VS, PS)
	P_RS_DSS_VP(P2, FrontCounterClockwise_True, DepthEnable_False, VS, PS)
}