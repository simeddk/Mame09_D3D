Texture2DArray<float4> Input;
RWTexture2DArray<float4> Output;


[numthreads(32, 32, 1)]
void CS(uint3 id : SV_DispatchThreadID)
{
	Output[id] = Input.Load(int4(id, 0));
}

[numthreads(32, 32, 1)]
void CS_Inverse(uint3 id : SV_DispatchThreadID)
{
	Output[id] = 1 - Input.Load(int4(id, 0));
}

technique11 T0
{
	pass P0
	{
		SetVertexShader(NULL);
		SetPixelShader(NULL);

		SetComputeShader(CompileShader(cs_5_0, CS()));
	}

	pass P1
	{
		SetVertexShader(NULL);
		SetPixelShader(NULL);

		SetComputeShader(CompileShader(cs_5_0, CS_Inverse()));
	}
}