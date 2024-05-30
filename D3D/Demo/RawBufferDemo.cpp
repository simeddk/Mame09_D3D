#include "stdafx.h"
#include "RawBufferDemo.h"

void RawBufferDemo::Initialize()
{
	Shader* shader = new Shader(L"18_ByteAddress.fxo");

	UINT count = 10 * 8 * 3;

	struct OutputFromUAV
	{
		UINT GroupID[3];
		UINT GroupThreadID[3];
		UINT DispatchID[3];
		UINT GroupIndex;
	};

	RawBuffer* rawBuffer = new RawBuffer(nullptr, 0, sizeof(OutputFromUAV) * count);

	shader->AsUAV("Output")->SetUnorderedAccessView(rawBuffer->UAV());
	shader->Dispatch(0, 0, 1, 1, 1);

	OutputFromUAV* output = new OutputFromUAV[count];
	rawBuffer->CopyFromOutput(output);
}
