
//ByteAddressBuffer Input;
RWByteAddressBuffer Output;

struct ComputInput
{
	uint3 GroupID : SV_GroupID;
	uint3 GroupThreadID : SV_GroupThreadID;
	uint3 DispatchID : SV_DispatchID;
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

	uint adress = input.GroupIndex * 10 * 4;
	
	Output.Store3(adress, asuint(output.GroupID));
	Output.Store3(adress + 12, asuint(output.GroupThreadID));
	Output.Store3(adress + 24, asuint(output.DispatchID));
	Output.Store3(adress + 36, asuint(output.GroupIndex));

}