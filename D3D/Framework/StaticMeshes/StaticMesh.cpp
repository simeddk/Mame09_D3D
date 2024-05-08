#include "Framework.h"
#include "StaticMesh.h"

StaticMesh::StaticMesh(Shader* shader)
	: shader(shader)
{
	D3DXMatrixIdentity(&world);

	sWorld = shader->AsMatrix("World");
	sView = shader->AsMatrix("View");
	sProjection = shader->AsMatrix("Projection");

	sDiffuseMap = shader->AsSRV("DiffuseMap");
}

StaticMesh::~StaticMesh()
{
	SafeDeleteArray(vertices);
	SafeDeleteArray(indices);

	SafeDelete(vertexBuffer);
	SafeDelete(indexBuffer);

	SafeDelete(diffuseMap);
}

void StaticMesh::Update()
{
}

void StaticMesh::Render()
{
	if (vertexBuffer == nullptr || indexBuffer == nullptr)
	{
		Create();
		
		vertexBuffer = new VertexBuffer(vertices, vertexCount, sizeof(VertexMesh));
		indexBuffer = new IndexBuffer(indices, indexCount);
	}

	vertexBuffer->IASet();
	indexBuffer->IASet();

	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	sWorld->SetMatrix(world);
	sView->SetMatrix(Context::Get()->View());
	sProjection->SetMatrix(Context::Get()->Projection());

	if (diffuseMap != nullptr)
		sDiffuseMap->SetResource(diffuseMap->SRV());

	shader->DrawIndexed(0, pass, indexCount);
}

void StaticMesh::Position(float x, float y, float z)
{
	Position(Vector3(x, y, z));
}

void StaticMesh::Position(Vector3& vec)
{
	position = vec;

	UpdateWorld();
}

void StaticMesh::Position(Vector3* vec)
{
	*vec = position;
}

void StaticMesh::Rotation(float x, float y, float z)
{
	Rotation(Vector3(x, y, z));
}

void StaticMesh::Rotation(Vector3& vec)
{
	rotation = vec;

	UpdateWorld();
}

void StaticMesh::Rotation(Vector3* vec)
{
	*vec = rotation;
}

void StaticMesh::RotationDegree(float x, float y, float z)
{
	RotationDegree(Vector3(x, y, z));
}

void StaticMesh::RotationDegree(Vector3& vec)
{
	rotation = vec * Math::PI / 180.f;

	UpdateWorld();
}

void StaticMesh::RotationDegree(Vector3* vec)
{
	*vec = rotation * 180.f / Math::PI;
}

void StaticMesh::Scale(float x, float y, float z)
{
	Scale(Vector3(x, y, z));
}

void StaticMesh::Scale(Vector3& vec)
{
	scale = vec;
	
	UpdateWorld();
}

void StaticMesh::Scale(Vector3* vec)
{
	*vec = scale;
}

Vector3 StaticMesh::Forward()
{
	Vector3 direction = Vector3(world._31, world._32, world._33);
	D3DXVec3Normalize(&direction, &direction);

	return direction;
}

Vector3 StaticMesh::Up()
{
	Vector3 direction = Vector3(world._21, world._22, world._23);
	D3DXVec3Normalize(&direction, &direction);

	return direction;
}

Vector3 StaticMesh::Right()
{
	Vector3 direction = Vector3(world._11, world._12, world._13);
	D3DXVec3Normalize(&direction, &direction);

	return direction;
}

void StaticMesh::DiffuseMap(wstring file)
{
	SafeDelete(diffuseMap);
	
	diffuseMap = new Texture(file);
}

void StaticMesh::UpdateWorld()
{
	Matrix S, R, T;
	D3DXMatrixScaling(&S, scale.x, scale.y, scale.z);
	D3DXMatrixRotationYawPitchRoll(&R, rotation.y, rotation.x, rotation.z);
	D3DXMatrixTranslation(&T, position.x, position.y, position.z);

	world = S * R * T;
}
