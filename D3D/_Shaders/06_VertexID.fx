matrix World, View, Projection;

struct VertexInput
{
	float4 Position : Position;
	uint VertexID : SV_VertexID;
};

struct VertexOutput
{
	float4 Position : SV_Position;
	float3 Color : Color;
};

VertexOutput VS(VertexInput input)
{
	VertexOutput output;
	output.Position = mul(input.Position, World);
	output.Position = mul(output.Position, View);
	output.Position = mul(output.Position, Projection);
	
	float3 vertexColor[] = { float3(1, 1, 1), float3(1, 1, 1), float3(1, 0, 0), float3(1, 1, 1) };
	output.Color = vertexColor[input.VertexID];
	
	return output;
}

float4 PS(VertexOutput input) : SV_Target
{
	return float4(input.Color, 1);
}

RasterizerState FillMode_WireFrame
{
	FillMode = WireFrame;
};

technique11 T0
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}

	pass P1
	{
		SetRasterizerState(FillMode_WireFrame);
		
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
}