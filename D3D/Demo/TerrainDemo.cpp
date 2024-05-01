#include "stdafx.h"
#include "TerrainDemo.h"

void TerrainDemo::Initialize()
{
	Context::Get()->GetCamera()->RotationDegree(30, 0, 0);
	Context::Get()->GetCamera()->Position(136, 129, -170);
	((Freedom*)Context::Get()->GetCamera())->Speed(50, 10);

	shader = new Shader(L"10_Terrain.fxo");
	terrain = new Terrain(shader, L"Terrain/Gray256.png");
}

void TerrainDemo::Destroy()
{
	SafeDelete(shader);
	SafeDelete(terrain);
}

void TerrainDemo::Update()
{
	static int pass = shader->PassCount() - 1;
	ImGui::SliderInt("Pass", &pass, 0, shader->PassCount() - 1);

	terrain->Pass() = pass;

	terrain->Update();
}

void TerrainDemo::Render()
{
	terrain->Render();
}
