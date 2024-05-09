#include "Framework.h"
#include  "CubeMapMesh.h"

CubeMapMesh::CubeMapMesh(Shader* shader)
	: shader(shader)
{
	mesh = new StaticMesh_Sphere(shader, 0.5f);

	sSrv = shader->AsSRV("CubeMap");
}

CubeMapMesh::~CubeMapMesh()
{
	SafeDelete(mesh);
	SafeRelease(srv);
}

void CubeMapMesh::Texture(wstring ddsFile)
{
	SafeRelease(srv);


	ddsFile = L"../../_Textures/" + ddsFile;
	Check(D3DX11CreateShaderResourceViewFromFile(D3D::GetDevice(), ddsFile.c_str(), nullptr, nullptr, &srv, nullptr));
}

void CubeMapMesh::Update()
{
	mesh->Update();
}

void CubeMapMesh::Render()
{
	sSrv->SetResource(srv);

	mesh->Render();
}