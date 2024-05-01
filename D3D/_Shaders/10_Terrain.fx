matrix World, View, Projection;

struct VertexInput
{
	float4 Position : Position;
};

struct VertexOutput
{
	float4 Position : SV_Position;
};

//-----------------------------------------------------------------------------
// Solid Color
//-----------------------------------------------------------------------------
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
	return float4(0, 1, 0, 1);
}

RasterizerState FillMode_WireFrame
{
	FillMode = WireFrame;
};

//-----------------------------------------------------------------------------
// Height Color
//-----------------------------------------------------------------------------
struct VertexOutput_HeightColor
{
	float4 Position : SV_Position;
	float4 Color : Color;
	float3 wPosition : Position1;
};

float4 GetHeightColor(float height)
{
	if (height > 20.f)
		return float4(1, 1, 1, 1);
	
	if (height > 10.f)
		return float4(0, 0, 1, 1);
	
	if (height > 5.f)
		return float4(0, 1, 0, 1);
	
	return float4(0.3f, 0.3f, 0.3f, 1);
}

VertexOutput_HeightColor VS_HeightColor(VertexInput input)
{
	VertexOutput_HeightColor output;
	output.Position = mul(input.Position, World);
	output.wPosition = output.Position.xyz;
	output.Position = mul(output.Position, View);
	output.Position = mul(output.Position, Projection);

	return output;
}

float4 PS_HeightColor(VertexOutput_HeightColor input) : SV_Target
{
	return GetHeightColor(input.wPosition.y);
}


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

	pass P2
	{
		SetVertexShader(CompileShader(vs_5_0, VS_HeightColor()));
		SetPixelShader(CompileShader(ps_5_0, PS_HeightColor()));
	}
	
}