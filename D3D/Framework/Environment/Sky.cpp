#include "Framework.h"
#include "Sky.h"

Sky::Sky(wstring ddsFile)
{
	shader = new Shader(L"15_Sky.fxo");
	sphere = new StaticMesh_Sphere(shader, 0.5f);

	ddsFile = L"../../_Textures/" + ddsFile;
	Check(D3DX11CreateShaderResourceViewFromFile(D3D::GetDevice(), ddsFile.c_str(), nullptr, nullptr, &srv, nullptr));

	sSrv = shader->AsSRV("SkyMap");
}

Sky::~Sky()
{
	SafeDelete(shader);
	SafeDelete(sphere);
	
	SafeRelease(srv);
}

void Sky::Update()
{
	Vector3 position;
	Context::Get()->GetCamera()->Position(&position);
	sphere->GetTransform()->Position(position);

	sphere->Update();
}

void Sky::Render()
{
	sSrv->SetResource(srv);

	sphere->Render();
}
