//Parameters
matrix World, View, Projection;
float3 LightDirection;
TextureCube CubeMap;

//Stats
SamplerState LinearSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

RasterizerState FillMode_WireFrame
{
	FillMode = WireFrame;
};

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
	output.Position = mul(input.Position, World);
	output.Position = mul(output.Position, View);
	output.Position = mul(output.Position, Projection);
	
	output.Uvw = normalize(input.Position.xyz - float3(0, 0, 0));
	
	return output;
}

float4 PS(VertexOutput input) : SV_Target
{
	return CubeMap.Sample(LinearSampler, input.Uvw);
}

technique11 T0
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}

}