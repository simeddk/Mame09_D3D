#include "stdafx.h"
#include "PolygonDemo.h"

void PolygonDemo::Initialize()
{
	shader = new Shader(L"03_Pass.fx");

	//Polygon1
	vertices[0].Position = Vector3(-0.5f, 0.0f, 0.f);
	vertices[1].Position = Vector3(+0.0f, 0.5f, 0.f);
	vertices[2].Position = Vector3(+0.5f, 0.0f, 0.f);

	//Polygon2
	vertices2[0].Position = Vector3(-0.5f, 0.0f - 0.5f, 0.f);
	vertices2[1].Position = Vector3(+0.0f, 0.5f - 0.5f, 0.f);
	vertices2[2].Position = Vector3(+0.5f, 0.0f - 0.5f, 0.f);

	//Create VertexBuffer1
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.ByteWidth = sizeof(Vertex) * 3;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA subResource = { 0 };
		subResource.pSysMem = vertices;

		Check(D3D::GetDevice()->CreateBuffer(&desc, &subResource, &vertexBuffer));
	}

	//Create VertexBuffer2
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.ByteWidth = sizeof(Vertex) * 3;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA subResource = { 0 };
		subResource.pSysMem = vertices2;

		Check(D3D::GetDevice()->CreateBuffer(&desc, &subResource, &vertexBuffer2));
	}
}

void PolygonDemo::Destroy()
{
	SafeDelete(shader);
	SafeRelease(vertexBuffer);
	SafeRelease(vertexBuffer2);
}

void PolygonDemo::Update()
{
}

void PolygonDemo::Render()
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	shader->Draw(0, 0, 3);

	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer2, &stride, &offset);

	static Color baseColor = Color(0.72f, 0.23f, 0.123f, 1.f);
	ImGui::ColorEdit3("BaseColor", baseColor);
	shader->AsVector("BaseColor")->SetFloatVector(baseColor);

	shader->Draw(0, 3, 3);
}
