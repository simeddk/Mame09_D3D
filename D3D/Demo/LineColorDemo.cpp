#include "stdafx.h"
#include "LineColorDemo.h"

void LineColorDemo::Initialize()
{
	shader = new Shader(L"02_LineColor.fx");

	vertices[0].Position = Vector3(0, 0, 0);
	vertices[1].Position = Vector3(0.5f, 0, 0);

	vertices[0].Color = Color(1, 1, 0, 1);
	vertices[1].Color = Color(1, 0, 1, 1);

	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.ByteWidth = sizeof(Vertex) * 2;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA subResource = { 0 };
	subResource.pSysMem = vertices;

	Check(D3D::GetDevice()->CreateBuffer(&desc, &subResource, &vertexBuffer));
}

void LineColorDemo::Destroy()
{
	SafeRelease(vertexBuffer);
}

void LineColorDemo::Update()
{
}

void LineColorDemo::Render()
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	shader->Draw(0, 0, 2);
}
