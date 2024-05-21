#include "Framework.h"
#include "SkeletalMeshRenderer.h"

SkeletalMeshRenderer::SkeletalMeshRenderer(Shader* shader)
	: shader(shader)
{
	skeletalMesh = new SkeletalMesh();

	transform = new Transform(shader);
}

SkeletalMeshRenderer::~SkeletalMeshRenderer()
{
	SafeDelete(skeletalMesh);
	SafeDelete(transform);
}

void SkeletalMeshRenderer::Update()
{
	if (bRead == true)
	{
		bRead = false;

		for (SkeletalMesh_Mesh* mesh : skeletalMesh->Meshes())
			mesh->SetShader(shader);

		UpdateTransforms();
	}

	for (SkeletalMesh_Mesh* mesh : skeletalMesh->Meshes())
		mesh->Update();
}

void SkeletalMeshRenderer::Render()
{
	for (SkeletalMesh_Mesh* mesh : skeletalMesh->Meshes())
	{
		mesh->SetTransform(transform); //World
		mesh->Render();
	}
}

void SkeletalMeshRenderer::ReadMesh(wstring file)
{
	skeletalMesh->ReadMesh(file);
}

void SkeletalMeshRenderer::ReadMaterial(wstring file)
{
	skeletalMesh->ReadMaterial(file);

	bRead = true;
}

void SkeletalMeshRenderer::Pass(UINT pass)
{
	for (SkeletalMesh_Mesh* mesh : skeletalMesh->Meshes())
		mesh->Pass(pass);
}

void SkeletalMeshRenderer::UpdateTransforms()
{
	for (UINT i = 0; i < skeletalMesh->BoneCount(); i++)
	{
		SkeletalMesh_Bone* bone = skeletalMesh->BoneByIndex(i);
		transforms[i] = bone->Transform();
	}

	for (SkeletalMesh_Mesh* mesh : skeletalMesh->Meshes())
		mesh->Transforms(transforms);
}
