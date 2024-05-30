ByteAddressBuffer InputBuffer;
RWByteAddressBuffer OutputBuffer;

struct ComputInput
{
	uint3 GroupID : SV_GroupID;
	uint3 GroupThreadID : SV_GroupThreadID;
	uint3 DispatchID : SV_DispatchThreadID;
	uint GroupIndex : SV_GroupIndex;
};

struct ComputOutput
{
	uint3 GroupID;
	uint3 GroupThreadID;
	uint3 DispatchID;
	uint GroupIndex;
};

[numthreads(10, 8, 3)]
void CS(ComputInput input)
{
	ComputOutput output;
	output.GroupID = asuint(input.GroupID);
	output.GroupThreadID = asuint(input.GroupThreadID);
	output.DispatchID = asuint(input.DispatchID);
	output.GroupIndex = asuint(input.GroupIndex);

	uint inAddress = (input.GroupID.x * 240 + input.GroupIndex) * 4;
	float retVal = asfloat(InputBuffer.Load(inAddress)) * 0.5f;
	
	uint outAdress = (input.GroupID.x * 240 + input.GroupIndex) * 11 * 4;
	OutputBuffer.Store3(outAdress, asuint(output.GroupID));
	OutputBuffer.Store3(outAdress + 12, asuint(output.GroupThreadID));
	OutputBuffer.Store3(outAdress + 24, asuint(output.DispatchID));
	OutputBuffer.Store(outAdress + 36, asuint(output.GroupIndex));
	OutputBuffer.Store(outAdress + 40, asuint(retVal));
}

technique11 T0
{
	pass P0
	{
		SetVertexShader(NULL);
		SetPixelShader(NULL);

		SetComputeShader(CompileShader(cs_5_0, CS()));
	}
}