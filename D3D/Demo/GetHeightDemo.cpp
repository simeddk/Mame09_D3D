#include "stdafx.h"
#include "GetHeightDemo.h"

void GetHeightDemo::Initialize()
{
	Context::Get()->GetCamera()->RotationDegree(30, 0, 0);
	Context::Get()->GetCamera()->Position(136, 129, -170);
	((Freedom*)Context::Get()->GetCamera())->Speed(50, 10);

	shader = new Shader(L"10_Terrain.fxo");
	terrain = new Terrain(shader, L"Terrain/Gray256.png");
	terrain->BaseMap(L"Terrain/Dirt2.png");
	terrain->LayerMap(L"Terrain/Cliff (Layered Rock).jpg");
	terrain->AlphaMap(L"Terrain/Gray256.png");

	triShader = new Shader(L"12_Tri.fxo");
	Vertex vertices[3];
	vertices[0].Position = Vector3(0, 1, 0);
	vertices[1].Position = Vector3(-1, 0, 0);
	vertices[2].Position = Vector3(+1, 0, 0);

	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.ByteWidth = sizeof(Vertex) * 3;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA subRessource = { 0 };
	subRessource.pSysMem = vertices;

	Check(D3D::GetDevice()->CreateBuffer(&desc, &subRessource, &vertexBuffer));

}

void GetHeightDemo::Destroy()
{
	SafeDelete(shader);
	SafeDelete(terrain);

	SafeDelete(triShader);
	SafeRelease(vertexBuffer);
}

void GetHeightDemo::Update()
{
	//Terrain Paramters
	{
		static int pass = shader->PassCount() - 1;
		ImGui::SliderInt("Pass", &pass, 0, shader->PassCount() - 1);
		terrain->Pass() = pass;

		static float tile = 4.f;
		ImGui::SliderFloat("Tile", &tile, 1, 10);
		terrain->Tile(tile);

		static float intensity = 2.5f;
		ImGui::SliderFloat("Intensity", &intensity, 1, 10);
		terrain->Intensity(intensity);
	}

	//Tri Key Input
	{
		if (Keyboard::Get()->Press(VK_RIGHT))
			position.x += 20.f * Time::Delta();
		else if (Keyboard::Get()->Press(VK_LEFT))
			position.x -= 20.f * Time::Delta();

		if (Keyboard::Get()->Press(VK_UP))
			position.z += 20.f * Time::Delta();
		else if (Keyboard::Get()->Press(VK_DOWN))
			position.z -= 20.f * Time::Delta();

		//position.y = terrain->GetHeightByInterp(position) + 1;
		position.y = terrain->GetHeightByRaycast(position) + 1;


		Matrix R, T;
		D3DXMatrixRotationX(&R, D3DX_PI);
		D3DXMatrixTranslation(&T, position.x, position.y, position.z);
		Matrix world = R * T;

		triShader->AsMatrix("World")->SetMatrix(world);
		triShader->AsMatrix("View")->SetMatrix(Context::Get()->View());
		triShader->AsMatrix("Projection")->SetMatrix(Context::Get()->Projection());
	}


	terrain->Update();
}

void GetHeightDemo::Render()
{
	terrain->Render();

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	triShader->Draw(0, 0, 3);

	string str = to_string(position.x) + ", " + to_string(position.y) + ", " + to_string(position.z);
	Gui::Get()->RenderText(5, 60, str);
}
