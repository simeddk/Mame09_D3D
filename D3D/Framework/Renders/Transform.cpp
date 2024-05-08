#include "Framework.h"
#include "Transform.h"

Transform::Transform()
	: shader(nullptr)
	, buffer(nullptr)
	, position(0, 0, 0)
	, scale(1, 1, 1)
	, rotation(0, 0, 0)
{
	D3DXMatrixIdentity(&bufferDesc.world);
}

Transform::Transform(Shader* shader)
	: position(0, 0, 0)
	, scale(1, 1, 1)
	, rotation(0, 0, 0)
{
	SetShader(shader);

	D3DXMatrixIdentity(&bufferDesc.world);
}

Transform::~Transform()
{
	SafeDelete(buffer);
}

void Transform::Set(Transform* transform)
{
	position = transform->position;
	scale = transform->scale;
	rotation = transform->rotation;

	UpdateWorld();
}

void Transform::SetShader(Shader* shader)
{
	this->shader = shader;

	buffer = new ConstantBuffer(&bufferDesc, sizeof(BufferDesc));
	sBuffer = shader->AsConstantBuffer("CB_World");
}

void Transform::Position(float x, float y, float z)
{
	Position(Vector3(x, y, z));
}

void Transform::Position(Vector3& vec)
{
	position = vec;

	UpdateWorld();
}

void Transform::Position(Vector3* vec)
{
	*vec = position;
}

void Transform::Scale(float x, float y, float z)
{
	Scale(Vector3(x, y, z));
}

void Transform::Scale(Vector3& vec)
{
	scale = vec;

	UpdateWorld();
}

void Transform::Scale(Vector3* vec)
{
	*vec = scale;
}

void Transform::Rotation(float x, float y, float z)
{
	Rotation(Vector3(x, y, z));
}

void Transform::Rotation(Vector3& vec)
{
	rotation = vec;

	UpdateWorld();
}

void Transform::Rotation(Vector3* vec)
{
	*vec = rotation;
}

void Transform::RotationDegree(float x, float y, float z)
{
	RotationDegree(Vector3(x, y, z));
}

void Transform::RotationDegree(Vector3& vec)
{
	Vector3 radian;
	radian.x = Math::ToRadian(vec.x);
	radian.y = Math::ToRadian(vec.y);
	radian.z = Math::ToRadian(vec.z);

	Rotation(radian);
}

void Transform::RotationDegree(Vector3* vec)
{
	Vector3 degree;
	degree.x = Math::ToDegree(rotation.x);
	degree.y = Math::ToDegree(rotation.y);
	degree.z = Math::ToDegree(rotation.z);

	*vec = degree;
}

Vector3 Transform::Forward()
{
	Vector3 forward = Vector3(bufferDesc.world._31, bufferDesc.world._32, bufferDesc.world._33);
	D3DXVec3Normalize(&forward, &forward);

	return forward;
}

Vector3 Transform::Up()
{
	Vector3 up = Vector3(bufferDesc.world._21, bufferDesc.world._22, bufferDesc.world._23);
	D3DXVec3Normalize(&up, &up);

	return up;
}

Vector3 Transform::Right()
{
	Vector3 right = Vector3(bufferDesc.world._11, bufferDesc.world._12, bufferDesc.world._13);
	D3DXVec3Normalize(&right, &right);

	return right;
}

void Transform::World(Matrix& matrix)
{
	Math::MatrixDecompose(bufferDesc.world, scale, rotation, position);

	bufferDesc.world = matrix;
}

void Transform::UpdateWorld()
{
	Matrix S, R, T;
	D3DXMatrixScaling(&S, scale.x, scale.y, scale.z);
	D3DXMatrixRotationYawPitchRoll(&R, rotation.y, rotation.x, rotation.z);
	D3DXMatrixTranslation(&T, position.x, position.y, position.z);

	bufferDesc.world = S * R * T;
}

void Transform::Update()
{
}

void Transform::Render()
{
	if (shader == nullptr) return;

	buffer->Map();
	sBuffer->SetConstantBuffer(buffer->Buffer());
}
