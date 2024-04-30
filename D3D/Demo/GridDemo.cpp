#include "stdafx.h"
#include "GridDemo.h"

void GridDemo::Initialize()
{
	Context::Get()->GetCamera()->RotationDegree(17, -1, 0);
	Context::Get()->GetCamera()->Position(58, 28, -21);
	Freedom* freedom = dynamic_cast<Freedom*>(Context::Get()->GetCamera());
	if (freedom != nullptr)
		freedom->Speed(10, 10);

	shader = new Shader(L"07_Grid.fxo");

	D3DXMatrixIdentity(&world);

	vertexCount = (width + 1) * (height + 1);
	vertices = new Vertex[vertexCount];

	//Grid
	for (UINT y = 0; y <= height; y++)
	{
		for (UINT x = 0; x <= width; x++)
		{
			UINT index = (width + 1) * y + x;

			vertices[index].Position.x = (float)x;
			vertices[index].Position.y = 0.f;
			vertices[index].Position.z = (float)y;
		}
	}

	//Create VertexBuffer
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.ByteWidth = sizeof(Vertex) * vertexCount;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA subResource = { 0 };
		subResource.pSysMem = vertices;

		Check(D3D::GetDevice()->CreateBuffer(&desc, &subResource, &vertexBuffer));
	}

	//Index Order
	indexCount = width * height * 6;
	indices = new UINT[indexCount];

	UINT index = 0;
	for (UINT y = 0; y < height; y++)
	{
		for (UINT x = 0; x < width; x++)
		{
			indices[index + 0] = (width + 1) * y + x;
			indices[index + 1] = (width + 1) * (y + 1) + x;
			indices[index + 2] = (width + 1) * y  + (x + 1);
			indices[index + 3] = (width + 1) * y  + (x + 1);
			indices[index + 4] = (width + 1) * (y + 1) + x;
			indices[index + 5] = (width + 1) * (y + 1) + (x + 1);

			index += 6;
		}
	}

	//Create IndexBuffer
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.ByteWidth = sizeof(UINT) * indexCount;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		D3D11_SUBRESOURCE_DATA subResource = { 0 };
		subResource.pSysMem = indices;

		Check(D3D::GetDevice()->CreateBuffer(&desc, &subResource, &indexBuffer));
	}

}

void GridDemo::Destroy()
{
	SafeDelete(shader);

	SafeRelease(vertexBuffer);
	SafeRelease(indexBuffer);

	SafeDeleteArray(vertices);
	SafeDeleteArray(indices);
}

void GridDemo::Update()
{
	shader->AsMatrix("World")->SetMatrix(world);
	shader->AsMatrix("View")->SetMatrix(Context::Get()->View());
	shader->AsMatrix("Projection")->SetMatrix(Context::Get()->Projection());
}

void GridDemo::Render()
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	

	static bool bWireFrame;
	ImGui::Checkbox("WireFrame", &bWireFrame);

	shader->DrawIndexed(0, bWireFrame ? 1 : 0, indexCount);
}
