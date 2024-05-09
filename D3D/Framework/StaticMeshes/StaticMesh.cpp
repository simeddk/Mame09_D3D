#include "Framework.h"
#include "StaticMesh.h"

StaticMesh::StaticMesh(Shader* shader)
	: StaticMeshRenderer(shader)
{
	sDiffuseMap = shader->AsSRV("DiffuseMap");
}

StaticMesh::~StaticMesh()
{
	SafeDeleteArray(vertices);
	SafeDeleteArray(indices);

	SafeDelete(diffuseMap);
}

void StaticMesh::Update()
{
	Super::Update();
}

void StaticMesh::Render()
{
	if (vertexBuffer == nullptr || indexBuffer == nullptr)
	{
		Create();
		
		vertexBuffer = new VertexBuffer(vertices, vertexCount, sizeof(VertexMesh));
		indexBuffer = new IndexBuffer(indices, indexCount);
	}

	Super::Render();

	if (diffuseMap != nullptr)
		sDiffuseMap->SetResource(diffuseMap->SRV());

	shader->DrawIndexed(0, Pass(), indexCount);
}

void StaticMesh::DiffuseMap(wstring file)
{
	SafeDelete(diffuseMap);
	
	diffuseMap = new Texture(file);
}