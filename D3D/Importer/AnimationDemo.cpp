#include "stdafx.h"
#include "AnimationDemo.h"

void AnimationDemo::Initialize()
{
	Context::Get()->GetCamera()->RotationDegree(12, 0, 0);
	Context::Get()->GetCamera()->Position(2, 4, -12);
	shader = new Shader(L"17_Animation.fxo");

	Kachujin();
}

void AnimationDemo::Destroy()
{
	SafeDelete(shader);

	SafeDelete(kachujin);
}

void AnimationDemo::Update()
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
	if (kachujin != nullptr)
	{	
		kachujin->Pass(pass);
		kachujin->Update();
	}
}

void AnimationDemo::Render()
{
	if (kachujin != nullptr)
		kachujin->Render();
}

void AnimationDemo::Kachujin()
{
	kachujin = new SkeletalMeshAnimator(shader);
	kachujin->ReadMesh(L"Kachujin/Mesh");
	kachujin->ReadMaterial(L"Kachujin/Mesh");
	kachujin->ReadClip(L"Kachujin/Idle");

	kachujin->GetTransform()->Position(0, 0, 0);
	kachujin->GetTransform()->Scale(0.01f, 0.01f, 0.01f);
}
