#include "stdafx.h"
#include "SamplerStateDemo.h"

void SamplerStateDemo::Initialize()
{
	Context::Get()->GetCamera()->Position(0, 0, -5);

	shader = new Shader(L"09_SamplerState.fxo");
	texture = new Texture(L"Box.png");

	D3DXMatrixIdentity(&world);

	vertices = new Vertex[4];

	vertices[0].Position = Vector3(-0.5f, -0.5f, 0.f);
	vertices[1].Position = Vector3(-0.5f, +0.5f, 0.f);
	vertices[2].Position = Vector3(+0.5f, -0.5f, 0.f);
	vertices[3].Position = Vector3(+0.5f, +0.5f, 0.f);

	vertices[0].Uv = Vector2(0, 1);
	vertices[1].Uv = Vector2(0, 0);
	vertices[2].Uv = Vector2(1, 1);
	vertices[3].Uv = Vector2(1, 0);

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

	indices = new UINT[6]{ 0, 1, 2, 2, 1, 3 };

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

void SamplerStateDemo::Destroy()
{
	SafeDelete(shader);

	SafeRelease(vertexBuffer);
	SafeRelease(indexBuffer);

	SafeDeleteArray(vertices);
	SafeDeleteArray(indices);
}

void SamplerStateDemo::Update()
{
	if (ImGui::Button("Load Texture"))
	{
		function<void(wstring)> onOpenCompleted = bind(&SamplerStateDemo::LoadTexture, this, placeholders::_1);
		Path::OpenFileDialog(L"", Path::ImageFilter, L"../../_Textures/", onOpenCompleted, D3D::GetDesc().Handle);
	}
	
	static UINT filter;
	ImGui::SliderInt("Filter", (int*)&filter, 0, 1);
	shader->AsScalar("Filter")->SetInt(filter);

	static UINT tile = 1;
	ImGui::SliderInt("Tile", (int*)&tile, 1, 5);
	shader->AsScalar("Tile")->SetInt(tile);

	static UINT address = 0;
	ImGui::SliderInt("Address", (int*)&address, 0, 3);
	shader->AsScalar("Address")->SetInt(address);

}

void SamplerStateDemo::Render()
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	
	shader->AsMatrix("World")->SetMatrix(world);
	shader->AsMatrix("View")->SetMatrix(Context::Get()->View());
	shader->AsMatrix("Projection")->SetMatrix(Context::Get()->Projection());

	shader->AsSRV("DiffuseMap")->SetResource(texture->SRV());

	static int pass = 0;
	ImGui::SliderInt("Pass", &pass, 0, 1);
	shader->DrawIndexed(0, pass, 6);
}

void SamplerStateDemo::LoadTexture(wstring path)
{
	SafeDelete(texture);

	texture = new Texture(path);
}