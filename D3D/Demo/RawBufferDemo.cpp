#include "stdafx.h"
#include "RawBufferDemo.h"

void RawBufferDemo::Initialize()
{
	Shader* shader = new Shader(L"18_ByteAddress.fxo");

	UINT count = 2 * 10 * 8 * 3;

	struct OutputFromUAV
	{
		UINT GroupID[3];		//ParentThread(C++)
		UINT GroupThreadID[3];	//Local - Vec3
		UINT DispatchID[3];		//Global - Vec3
		UINT GroupIndex;		//Local - Scalar
		float RetVal;			//CS Result
	};

	float* data = new float[count];
	for (UINT i = 0; i < count; i++)
		data[i] = i * 100 + 100;

	RawBuffer* rawBuffer = new RawBuffer(data, sizeof(float) * count, sizeof(OutputFromUAV) * count);
	
	shader->AsSRV("InputBuffer")->SetResource(rawBuffer->SRV());
	shader->AsUAV("OutputBuffer")->SetUnorderedAccessView(rawBuffer->UAV());
	shader->Dispatch(0, 0, 2, 1, 1);

	OutputFromUAV* output = new OutputFromUAV[count];
	rawBuffer->CopyFromOutput(output);

	FILE* fileBuffer;
	fopen_s(&fileBuffer, "../RawBuffer.csv", "w");
	for (UINT i = 0 ; i < count; i++)
	{
		OutputFromUAV temp = output[i];

		fprintf
		(
			fileBuffer,
			"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%f\n",
			temp.GroupID[0], temp.GroupID[1], temp.GroupID[2], //GroupID
			temp.GroupThreadID[0], temp.GroupThreadID[1], temp.GroupThreadID[2], //GroupThreadID
			temp.DispatchID[0], temp.DispatchID[1], temp.DispatchID[2], //DispatchID
			temp.GroupIndex, //GroupIndex
			temp.RetVal //Cs Result
		);
	}
	fclose(fileBuffer);
}
