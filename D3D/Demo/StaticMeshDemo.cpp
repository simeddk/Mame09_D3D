#include "stdafx.h"
#include "StaticMeshDemo.h"

void StaticMeshDemo::Initialize()
{
	Context::Get()->GetCamera()->RotationDegree(20, 0, 0);
	Context::Get()->GetCamera()->Position(0, 36, -85);

	shader = new Shader(L"13_StaticMesh.fxo");
	sDirection = shader->AsVector("LightDirection");

	CreateStaticMesh();
}

void StaticMeshDemo::Destroy()
{
	SafeDelete(shader);
	
	SafeDelete(quad);
	SafeDelete(plane);

	SafeDelete(cube);
	
	for (UINT i = 0; i < 10; i++)
	{
		SafeDelete(spheres[i]);
		SafeDelete(cylinders[i]);
	}
}

void StaticMeshDemo::Update()
{
	ImGui::SliderFloat3("Light Direction", direction, -1, 1);
	sDirection->SetFloatVector(direction);

	static bool bWire;
	ImGui::Checkbox("Wire Frame", &bWire);

	quad->Pass(bWire ? 1 : 0);
	plane->Pass(bWire ? 1 : 0);
	cube->Pass(bWire ? 1 : 0);

	quad->Update();
	plane->Update();

	cube->Update();

	for (UINT i = 0; i < 10; i++)
	{
		cylinders[i]->Pass(bWire ? 1 : 0);
		cylinders[i]->Update();

		spheres[i]->Pass(bWire ? 1 : 0);
		spheres[i]->Update();
	}
}

void StaticMeshDemo::Render()
{
	quad->Render();
	plane->Render();

	cube->Render();

	for (UINT i = 0; i < 10; i++)
	{
		cylinders[i]->Render();
		spheres[i]->Render();
	}
}

void StaticMeshDemo::CreateStaticMesh()
{
	quad = new StaticMesh_Quad(shader);
	quad->DiffuseMap(L"Box.png");

	plane = new StaticMesh_Plane(shader, 2.5f, 2.5f);
	plane->GetTransform()->Scale(12, 1, 12);
	plane->DiffuseMap(L"Floor.png");

	cube = new StaticMesh_Cube(shader);
	cube->GetTransform()->Position(0, 5, 0);
	cube->GetTransform()->Scale(20, 10, 20);
	cube->DiffuseMap(L"Stones.png");

	for (UINT i = 0; i < 5; i++)
	{
		//Cylinder - Left
		cylinders[i * 2 + 0] = new StaticMesh_Cylinder(shader, 0.3f, 0.5f, 3.f, 20, 20);
		cylinders[i * 2 + 0]->GetTransform()->Position(-30, 6, (float)i * 15.f - 15.f);
		cylinders[i * 2 + 0]->GetTransform()->Scale(5, 5, 5);
		cylinders[i * 2 + 0]->DiffuseMap(L"Bricks.png");

		//Cylinder - Right
		cylinders[i * 2 + 1] = new StaticMesh_Cylinder(shader, 0.3f, 0.5f, 3.f, 20, 20);
		cylinders[i * 2 + 1]->GetTransform()->Position(+30, 6, (float)i * 15.f - 15.f);
		cylinders[i * 2 + 1]->GetTransform()->Scale(5, 5, 5);
		cylinders[i * 2 + 1]->DiffuseMap(L"Bricks.png");

		//Sphere - Left
		spheres[i * 2 + 0] = new StaticMesh_Sphere(shader, 0.5f);
		spheres[i * 2 + 0]->GetTransform()->Position(-30, 15, (float)i * 15.f - 15.f);
		spheres[i * 2 + 0]->GetTransform()->Scale(5, 5, 5);
		spheres[i * 2 + 0]->DiffuseMap(L"Wall.png");

		//Sphere - Right
		spheres[i * 2 + 1] = new StaticMesh_Sphere(shader, 0.5f);
		spheres[i * 2 + 1]->GetTransform()->Position(+30, 15, (float)i * 15.f - 15.f);
		spheres[i * 2 + 1]->GetTransform()->Scale(5, 5, 5);
		spheres[i * 2 + 1]->DiffuseMap(L"Wall.png");
	}
}
