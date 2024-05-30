#include "Framework.h"
#include "Collider.h"

//-----------------------------------------------------------------------------
//ColliderObject
//-----------------------------------------------------------------------------
ColliderObject::ColliderObject()
{
	Init = new Transform();
	World = new Transform();
	Collision = new Collider(World, Init);
}

ColliderObject::~ColliderObject()
{
	SafeDelete(Init);
	SafeDelete(World);
	SafeDelete(Collision);
}


//-----------------------------------------------------------------------------
//Collider
//-----------------------------------------------------------------------------
Collider::Collider(Transform* transform, Transform* init)
	: transform(transform), init(init)
{
	lines[0] = Vector3(-0.5f, -0.5f, -0.5f); //좌하(근)
	lines[1] = Vector3(-0.5f, +0.5f, -0.5f); //좌상(근)
	lines[2] = Vector3(+0.5f, -0.5f, -0.5f); //우하(근)
	lines[3] = Vector3(+0.5f, +0.5f, -0.5f); //우상(근)

	lines[4] = Vector3(-0.5f, -0.5f, +0.5f); //좌하(원)
	lines[5] = Vector3(-0.5f, +0.5f, +0.5f); //좌상(원)
	lines[6] = Vector3(+0.5f, -0.5f, +0.5f); //우하(원)
	lines[7] = Vector3(+0.5f, +0.5f, +0.5f); //우상(원)
}

Collider::~Collider()
{

}

void Collider::Update()
{

}

void Collider::Render(Color color)
{
	Vector3 dest[8];

	Transform temp;
	temp.World(transform->World());

	if (init != nullptr)
		temp.World(init->World() * transform->World());

	Matrix world = temp.World();

	for (UINT i = 0; i < 8; i++)
		D3DXVec3TransformCoord(&dest[i], &lines[i], &world);


	//Front
	DebugLine::Get()->RenderLine(dest[0], dest[1], color);
	DebugLine::Get()->RenderLine(dest[1], dest[3], color);
	DebugLine::Get()->RenderLine(dest[3], dest[2], color);
	DebugLine::Get()->RenderLine(dest[2], dest[0], color);

	//Backward
	DebugLine::Get()->RenderLine(dest[4], dest[5], color);
	DebugLine::Get()->RenderLine(dest[5], dest[7], color);
	DebugLine::Get()->RenderLine(dest[7], dest[6], color);
	DebugLine::Get()->RenderLine(dest[6], dest[4], color);

	//Side
	DebugLine::Get()->RenderLine(dest[0], dest[4], color);
	DebugLine::Get()->RenderLine(dest[1], dest[5], color);
	DebugLine::Get()->RenderLine(dest[2], dest[6], color);
	DebugLine::Get()->RenderLine(dest[3], dest[7], color);
}