#include "Framework.h"
#include "SkeletalMeshMesh.h"

SkeletalMesh_Mesh::SkeletalMesh_Mesh()
{
	boneBuffer = new ConstantBuffer(&boneDesc, sizeof(BoneDesc));
}

SkeletalMesh_Mesh::~SkeletalMesh_Mesh()
{
	SafeDelete(transform);
	SafeDelete(perFrame);

	SafeDeleteArray(vertices);
	SafeDeleteArray(indices);

	SafeDelete(vertexBuffer);
	SafeDelete(indexBuffer);

	SafeDelete(boneBuffer);

	for (SkeletalMesh_MeshPart* part : meshParts)
		SafeDelete(part);
}

void SkeletalMesh_Mesh::Binding(SkeletalMesh* skeletalMesh)
{
	vertexBuffer = new VertexBuffer(vertices, vertexCount, sizeof(SkeletalMesh::VertexSkeletalMesh));
	indexBuffer = new IndexBuffer(indices, indexCount);

	for (SkeletalMesh_MeshPart* part : meshParts)
		part->Binding(skeletalMesh);
}

void SkeletalMesh_Mesh::Pass(UINT val)
{
	for (SkeletalMesh_MeshPart* part : meshParts)
		part->Pass(val);
}

void SkeletalMesh_Mesh::SetShader(Shader* shader)
{
	this->shader = shader;

	SafeDelete(transform);
	transform = new Transform(shader);

	SafeDelete(perFrame);
	perFrame = new PerFrame(shader);

	sBoneBuffer = shader->AsConstantBuffer("CB_Bones");

	for (SkeletalMesh_MeshPart* part : meshParts)
		part->SetShader(shader);
}

void SkeletalMesh_Mesh::Update()
{
	boneDesc.BoneIndex = boneIndex;

	perFrame->Update();
	transform->Update();

	for (SkeletalMesh_MeshPart* part : meshParts)
		part->Update();
}

void SkeletalMesh_Mesh::Render()
{
	boneBuffer->Map();
	sBoneBuffer->SetConstantBuffer(boneBuffer->Buffer());

	perFrame->Render();
	transform->Render();

	vertexBuffer->IASet();
	indexBuffer->IASet();

	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	for (SkeletalMesh_MeshPart* part : meshParts)
		part->Render();
}

void SkeletalMesh_Mesh::Transforms(Matrix* transforms)
{
	memcpy(boneDesc.Transforms, transforms, sizeof(Matrix) * MAX_BONE_COUNT);
}

void SkeletalMesh_Mesh::SetTransform(Transform* trasnform)
{
	this->transform->Set(transform);
}

//-----------------------------------------------------------------------------
//SkeletalMesh_MeshPart
//-----------------------------------------------------------------------------
void SkeletalMesh_MeshPart::Update()
{
	
}

void SkeletalMesh_MeshPart::Render()
{
	shader->DrawIndexed(0, pass, indexCount, startIndex);
}

void SkeletalMesh_MeshPart::Binding(SkeletalMesh* skeletalMesh)
{
}

void SkeletalMesh_MeshPart::SetShader(Shader* shader)
{
	this->shader = shader;
}
