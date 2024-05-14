#include "stdafx.h"
#include "SkeletalMeshDemo.h"

void SkeletalMeshDemo::Initialize()
{
	shader = new Shader(L"16_SkeletalMesh.fxo");

	Tank();
	Kachujin();
}

void SkeletalMeshDemo::Destroy()
{
	SafeDelete(shader);

	SafeDelete(tank);
	SafeDelete(kachujin);
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
}

void SkeletalMeshDemo::Render()
{
	if (tank != nullptr)
		tank->Render();

	if (kachujin != nullptr)
		kachujin->Render();
}

void SkeletalMeshDemo::Tank()
{
	tank = new SkeletalMeshRenderer(shader);
	tank->ReadMesh(L"Tank/Tank");
}

void SkeletalMeshDemo::Kachujin()
{
	kachujin = new SkeletalMeshRenderer(shader);
	kachujin->ReadMesh(L"Kachujin/Mesh");

	kachujin->GetTransform()->Position(5, 0, 0);
	kachujin->GetTransform()->Scale(0.01f, 0.01f, 0.01f);
}
