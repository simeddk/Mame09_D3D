#include "stdafx.h"
#include "MultiLineDemo.h"

void MultiLineDemo::Initialize()
{
	shader = new Shader(L"03_Pass.fxo");

	//Line1
	vertices[0].Position = Vector3(0, 0, 0);
	vertices[1].Position = Vector3(0.5f, 0, 0);

	//Line2
	vertices[2].Position = Vector3(0, 0.5f, 0);
	vertices[3].Position = Vector3(0.5f, 0.5f, 0);

	//Line3
	vertices[4].Position = Vector3(0, -0.5f, 0);
	vertices[5].Position = Vector3(0.5f, -0.5f, 0);


	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.ByteWidth = sizeof(Vertex) * 6;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA subResource = { 0 };
	subResource.pSysMem = vertices;

	Check(D3D::GetDevice()->CreateBuffer(&desc, &subResource, &vertexBuffer));
}

void MultiLineDemo::Destroy()
{
	SafeDelete(shader);
	SafeRelease(vertexBuffer);
}

void MultiLineDemo::Update()
{
}

void MultiLineDemo::Render()
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	
	
	//PrimitiveTopology Test
	D3D11_PRIMITIVE_TOPOLOGY topology[] =
	{
		D3D11_PRIMITIVE_TOPOLOGY_LINELIST,
		D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP,
	};

	static bool bStrip;
	ImGui::Checkbox("Strip Mode", &bStrip);
	D3D::GetDC()->IASetPrimitiveTopology(bStrip ? topology[1] : topology[0]);

	//StartLocation Test
	static int startLocation = 0;
	ImGui::SliderInt("Start Location", &startLocation, 0, 6);

	//Pass Test
	static int pass;
	ImGui::InputInt("Pass", &pass);
	pass = Math::Clamp(pass, 0, 3);

	//VectorParam Test
	static Color baseColor = Color(1, 0, 1, 1);
	ImGui::ColorEdit3("Base Color", baseColor);
	shader->AsVector("BaseColor")->SetFloatVector(baseColor);

	shader->Draw(0, pass, 6, startLocation);
}
