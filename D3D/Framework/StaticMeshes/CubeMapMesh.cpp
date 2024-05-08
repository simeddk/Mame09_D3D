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

void CubeMapMesh::Position(float x, float y, float z)
{
	Position(Vector3(x, y, z));
}

void CubeMapMesh::Position(Vector3& vec)
{
	mesh->Position(vec);
}

void CubeMapMesh::Position(Vector3* vec)
{
	mesh->Position(vec);
}

void CubeMapMesh::Scale(float x, float y, float z)
{
	Scale(Vector3(x, y, z));
}

void CubeMapMesh::Scale(Vector3& vec)
{
	mesh->Scale(vec);
}

void CubeMapMesh::Scale(Vector3* vec)
{
	mesh->Scale(vec);
}
