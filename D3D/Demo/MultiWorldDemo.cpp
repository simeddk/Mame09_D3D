#include "stdafx.h"
#include "MultiWorldDemo.h"

void MultiWorldDemo::Initialize()
{
	shader = new Shader(L"05_World.fxo");

	D3DXMatrixIdentity(&world);

	//Plane
	vertices[0].Position = Vector3(-0.5f + 0.5f, -0.5f + 0.5f, 0.f);
	vertices[1].Position = Vector3(-0.5f + 0.5f, +0.5f + 0.5f, 0.f);
	vertices[2].Position = Vector3(+0.5f + 0.5f, -0.5f + 0.5f, 0.f);
	vertices[3].Position = Vector3(+0.5f + 0.5f, -0.5f + 0.5f, 0.f);
	vertices[4].Position = Vector3(-0.5f + 0.5f, +0.5f + 0.5f, 0.f);
	vertices[5].Position = Vector3(+0.5f + 0.5f, +0.5f + 0.5f, 0.f);

	//Create VertexBuffer
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.ByteWidth = sizeof(Vertex) * 6;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA subResource = { 0 };
		subResource.pSysMem = vertices;

		Check(D3D::GetDevice()->CreateBuffer(&desc, &subResource, &vertexBuffer));
	}
}

void MultiWorldDemo::Destroy()
{
	SafeDelete(shader);
	SafeRelease(vertexBuffer);
}

void MultiWorldDemo::Update()
{

}

void MultiWorldDemo::Render()
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	
	shader->AsMatrix("View")->SetMatrix(Context::Get()->View());
	shader->AsMatrix("Projection")->SetMatrix(Context::Get()->Projection());
	
	//T : 0,0,0 / S : 1,2,1
	Matrix S, T;
	D3DXMatrixScaling(&S, 1, 2, 1);
	D3DXMatrixTranslation(&T, 0, 0, 0);
	world = S * T;

	shader->AsMatrix("World")->SetMatrix(world);

	static bool bWireFrame;
	ImGui::Checkbox("WireFrame", &bWireFrame);

	shader->Draw(0, bWireFrame ? 1 : 0, 6);

	//T : 2, 2, 2 / S : 2, 1, 1
	D3DXMatrixScaling(&S, 2, 1, 1);
	D3DXMatrixTranslation(&T, 2, 2, 2);
	world = S * T;

	shader->AsMatrix("World")->SetMatrix(world);

	shader->Draw(0, bWireFrame ? 1 : 0, 6);
}
