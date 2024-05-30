#pragma once

//-----------------------------------------------------------------------------
//VertexBuffer
//-----------------------------------------------------------------------------
class VertexBuffer
{
public:
	VertexBuffer(void* data, UINT count, UINT stride, UINT slot = 0, bool bCpuWrite = false, bool bGpuWrite = false);
	~VertexBuffer();

	UINT Count() { return count; }
	UINT Stride() { return stride; }
	ID3D11Buffer* Buffer() { return buffer; }

	void IASet();

private:
	ID3D11Buffer* buffer;
	
	void* data;
	
	UINT count;
	UINT stride;
	UINT slot;

	bool bCpuWrite;
	bool bGpuWrite;
};

//-----------------------------------------------------------------------------
//IndexBuffer
//-----------------------------------------------------------------------------
class IndexBuffer
{
public:
	IndexBuffer(void* data, UINT count);
	~IndexBuffer();

	UINT Count() { return count; }
	ID3D11Buffer* Buffer() { return buffer; }

	void IASet();

private:
	ID3D11Buffer* buffer;

	void* data;
	UINT count;
};

//-----------------------------------------------------------------------------
//ConstantBuffer
//-----------------------------------------------------------------------------
class ConstantBuffer
{
public:
	ConstantBuffer(void* data, UINT dataSize);
	~ConstantBuffer();

	ID3D11Buffer* Buffer() {return buffer; }

	void Map();

private:
	ID3D11Buffer* buffer;

	void* data;
	UINT dataSize;
	
};

//-----------------------------------------------------------------------------
//CsResource(Super)
//-----------------------------------------------------------------------------
class CsResource
{
public:
	CsResource();
	virtual ~CsResource();

protected:
	virtual void CreateInput() {}
	virtual void CreateSRV() {}

	virtual void CreateOutput() {}
	virtual void CreateUAV() {}

	void CreateBuffer();

public:
	ID3D11ShaderResourceView* SRV() { return srv; }
	ID3D11UnorderedAccessView* UAV() { return uav; }

protected:
	ID3D11Resource* input = nullptr;
	ID3D11ShaderResourceView* srv = nullptr;

	ID3D11Resource* output = nullptr;
	ID3D11UnorderedAccessView* uav = nullptr;
};

//-----------------------------------------------------------------------------
//CsResource(Super)
//-----------------------------------------------------------------------------
class RawBuffer : public CsResource
{
public:
	RawBuffer(void* data, UINT inputByte, UINT outputByte);
	~RawBuffer();

private:
	virtual void CreateInput() override;
	virtual void CreateSRV() override;

	virtual void CreateOutput() override;
	virtual void CreateUAV() override;

public:
	void CopyToInput(void* data);
	void CopyFromOutput(void* data);

private:
	void* inputData;
	UINT inputByte;
	UINT outputByte;
};