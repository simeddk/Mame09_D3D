#include "Framework.h"
#include "StaticMeshRenderer.h"

StaticMeshRenderer::StaticMeshRenderer(Shader* shader)
	: shader(shader)
	, bCreatedShader(false)
{
	Initialize();
}

StaticMeshRenderer::StaticMeshRenderer(wstring shaderFile)
	: bCreatedShader(true)
{
	shader = new Shader(shaderFile);

	Initialize();
}

void StaticMeshRenderer::Initialize()
{
	transform = new Transform(shader);
	perFrame = new PerFrame(shader);
}

StaticMeshRenderer::~StaticMeshRenderer()
{
	SafeDelete(transform);
	SafeDelete(perFrame);

	SafeDelete(vertexBuffer);
	SafeDelete(indexBuffer);

	if (bCreatedShader == true)
		SafeDelete(shader);
}

void StaticMeshRenderer::Update()
{
	transform->Update();
	perFrame->Update();
}

void StaticMeshRenderer::Render()
{

	if (vertexBuffer != nullptr)
	{
		vertexBuffer->IASet();

		if (indexBuffer != nullptr)
			indexBuffer->IASet();
	}

	D3D::GetDC()->IASetPrimitiveTopology(topology);

	transform->Render();
	perFrame->Render();
}


