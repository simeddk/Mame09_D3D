#pragma once

class StaticMeshRenderer
{
public:
	StaticMeshRenderer(Shader* shader);
	StaticMeshRenderer(wstring shaderFile);
	virtual ~StaticMeshRenderer();

	Shader* GetShader() { return shader; }

	UINT& Pass() { return pass; }
	void Pass(UINT val) { pass = val; }

	virtual void Update();
	virtual void Render();

	Transform* GetTransform() { return transform; }

protected:
	void Topology(D3D11_PRIMITIVE_TOPOLOGY topology) { this->topology = topology; }

private:
	void Initialize();

protected:
	Shader* shader;

	Transform* transform;
	VertexBuffer* vertexBuffer = nullptr;
	IndexBuffer* indexBuffer = nullptr;

	UINT vertexCount = 0;
	UINT indexCount = 0;

private:
	bool bCreatedShader = false;

	D3D11_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	UINT pass = 0;

	PerFrame* perFrame;
};