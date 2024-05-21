#include "stdafx.h"
#include "SkeletalMeshDemo.h"

void SkeletalMeshDemo::Initialize()
{
	shader = new Shader(L"16_SkeletalMesh.fxo");

	Tank();
	Kachujin();
	Tower();
	Airplane();
}

void SkeletalMeshDemo::Destroy()
{
	SafeDelete(shader);

	SafeDelete(tank);
	SafeDelete(kachujin);
	SafeDelete(tower);
	SafeDelete(airplane);
}

void SkeletalMeshDemo::Update()
{
	//Light Direction Test
	static Vector3 lightDirection = Vector3(-1, -1, 1);
	ImGui::SliderFloat3("Light Direction", lightDirection, -1, 1);
	shader->AsVector("LightDirection")->SetFloatVector(lightDirection);

	//Pass Test
	static int pass = 0;
	ImGui::InputInt("Pass", &pass);
	pass %= 2;

	//Update
	if (tank != nullptr)
	{
		tank->Pass(pass);
		tank->Update();
	}

	if (kachujin != nullptr)
	{	
		kachujin->Pass(pass);
		kachujin->Update();
	}

	if (tower != nullptr)
	{
		tower->Pass(pass);
		tower->Update();
	}

	if (airplane != nullptr)
	{
		airplane->Pass(pass);
		airplane->Update();
	}
}

void SkeletalMeshDemo::Render()
{
	if (tank != nullptr)
		tank->Render();

	if (kachujin != nullptr)
		kachujin->Render();

	if (tower != nullptr)
		tower->Render();

	if (airplane != nullptr)
		airplane->Render();
}

void SkeletalMeshDemo::Tank()
{
	tank = new SkeletalMeshRenderer(shader);
	tank->ReadMesh(L"Tank/Tank");
	tank->ReadMaterial(L"Tank/Tank");
}

void SkeletalMeshDemo::Kachujin()
{
	kachujin = new SkeletalMeshRenderer(shader);
	kachujin->ReadMesh(L"Kachujin/Mesh");
	kachujin->ReadMaterial(L"Kachujin/Mesh");
	kachujin->GetTransform()->Position(5, 0, 0);
	kachujin->GetTransform()->Scale(0.01f, 0.01f, 0.01f);
}

void SkeletalMeshDemo::Tower()
{
	tower = new SkeletalMeshRenderer(shader);
	tower->ReadMesh(L"Tower/Tower");
	tower->ReadMaterial(L"Tower/Tower");
	tower->GetTransform()->Position(-5, 0, 0);
	tower->GetTransform()->Scale(0.01f, 0.01f, 0.01f);
}

void SkeletalMeshDemo::Airplane()
{
	airplane = new SkeletalMeshRenderer(shader);
	airplane->ReadMesh(L"B787/Airplane");
	airplane->ReadMaterial(L"B787/Airplane");
	airplane->GetTransform()->Position(-10, 0, 0);
	airplane->GetTransform()->Scale(0.001f, 0.001f, 0.001f);
}
