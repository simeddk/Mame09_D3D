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

//-----------------------------------------------------------------------------
// Lambert
//-----------------------------------------------------------------------------
struct VertexInput_Lambert
{
	float4 Position : Position;
	float3 Normal : Normal;
};

struct VertexOutput_Lambert
{
	float4 Position : SV_Position;
	float4 Color : Color;
	float3 wPosition : Position1;
	float3 Normal : Normal;
};

VertexOutput_Lambert VS_Lambert(VertexInput_Lambert input)
{
	VertexOutput_Lambert output;
	output.Position = mul(input.Position, World);
	output.wPosition = output.Position.xyz;
	output.Position = mul(output.Position, View);
	output.Position = mul(output.Position, Projection);

	output.Normal = mul(input.Normal, (float3x3)World);
	
	return output;
}

float3 LightDirection = float3(-1, -1, 1);
float4 PS_Lambert(VertexOutput_Lambert input) : SV_Target
{
	float3 normal = normalize(input.Normal);
	float lambert = saturate(dot(normal, -LightDirection));

	float4 baseColor = GetHeightColor(input.wPosition.y);
	
	return baseColor * lambert;
}

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
	output.Position = mul(input.Position, World);
	output.Position = mul(output.Position, View);
	output.Position = mul(output.Position, Projection);

	output.Normal = mul(input.Normal, (float3x3) World);
	
	output.Uv = input.Uv;
	
	return output;
}

Texture2D BaseMap;
Texture2D LayerMap;
Texture2D AlphaMap;

SamplerState LinearSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

float Tile = 1;
float Intensity = 1;

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

	pass P3
	{
		SetVertexShader(CompileShader(vs_5_0, VS_Lambert()));
		SetPixelShader(CompileShader(ps_5_0, PS_Lambert()));
	}

	pass P4
	{
		SetVertexShader(CompileShader(vs_5_0, VS_Texture()));
		SetPixelShader(CompileShader(ps_5_0, PS_Texture()));
	}
	
}