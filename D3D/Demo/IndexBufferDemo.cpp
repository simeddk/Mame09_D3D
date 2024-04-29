#include "stdafx.h"
#include "IndexBufferDemo.h"

void IndexBufferDemo::Initialize()
{
	shader = new Shader(L"06_VertexId.fxo");

	D3DXMatrixIdentity(&world);

	//Plane
	vertices[0].Position = Vector3(-0.5f, -0.5f, 0.f);
	vertices[1].Position = Vector3(-0.5f, +0.5f, 0.f);
	vertices[2].Position = Vector3(+0.5f, -0.5f, 0.f);
	vertices[3].Position = Vector3(+0.5f, +0.5f, 0.f);

	//Create VertexBuffer
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.ByteWidth = sizeof(Vertex) * 4;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA subResource = { 0 };
		subResource.pSysMem = vertices;

		Check(D3D::GetDevice()->CreateBuffer(&desc, &subResource, &vertexBuffer));
	}

	//Index Order
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 2;
	indices[4] = 1;
	indices[5] = 3;

	//Create IndexBuffer
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.ByteWidth = sizeof(UINT) * 6;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		D3D11_SUBRESOURCE_DATA subResource = { 0 };
		subResource.pSysMem = indices;

		Check(D3D::GetDevice()->CreateBuffer(&desc, &subResource, &indexBuffer));
	}
}

void IndexBufferDemo::Destroy()
{
	SafeDelete(shader);
	SafeRelease(vertexBuffer);
	SafeRelease(indexBuffer);
}

void IndexBufferDemo::Update()
{
	//Position
	static Vector3 position;

	if (Keyboard::Get()->Press('D'))
		position.x += 2.f * Time::Delta();
	else if (Keyboard::Get()->Press('A'))
		position.x -= 2.f * Time::Delta();

	if (Keyboard::Get()->Press('W'))
		position.y += 2.f * Time::Delta();
	else if (Keyboard::Get()->Press('S'))
		position.y -= 2.f * Time::Delta();

	//Scale
	static Vector3 scale = Vector3(1, 1, 1);

	if (Keyboard::Get()->Press(VK_RIGHT))
		scale.x += 2.f * Time::Delta();
	else if (Keyboard::Get()->Press(VK_LEFT))
		scale.x -= 2.f * Time::Delta();

	if (Keyboard::Get()->Press(VK_UP))
		scale.y += 2.f * Time::Delta();
	else if (Keyboard::Get()->Press(VK_DOWN))
		scale.y -= 2.f * Time::Delta();

	Matrix S, T;
	D3DXMatrixScaling(&S, scale.x, scale.y, scale.z);
	D3DXMatrixTranslation(&T, position.x, position.y, position.z);
	world = S * T;
}

void IndexBufferDemo::Render()
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	
	shader->AsMatrix("World")->SetMatrix(world);
	shader->AsMatrix("View")->SetMatrix(Context::Get()->View());
	shader->AsMatrix("Projection")->SetMatrix(Context::Get()->Projection());

	static bool bWireFrame;
	ImGui::Checkbox("WireFrame", &bWireFrame);

	shader->DrawIndexed(0, bWireFrame ? 1 : 0, 6);
}
