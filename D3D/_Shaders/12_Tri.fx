matrix World, View, Projection;

struct VertexInput
{
	float4 Position : Position;
};

struct VertexOutput
{
	float4 Position : SV_Position;
};

VertexOutput VS(VertexInput input)
{
	VertexOutput output;
	output.Position = mul(input.Position, World);
	output.Position = mul(output.Position, View);
	output.Position = mul(output.Position, Projection);
	
	return output;
}

float4 PS(VertexOutput input) : SV_Target
{
	return float4(1, 0, 0, 1);
}

RasterizerState CullMode_None
{
	CullMode = None;
};

technique11 T0
{
	pass P0
	{
		SetRasterizerState(CullMode_None);
		
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
}