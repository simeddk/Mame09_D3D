#include "Framework.h"
#include "Camera.h"

Camera::Camera()
{
	D3DXMatrixIdentity(&matRotation);
	D3DXMatrixIdentity(&matView);

	Rotation();
	Move();
}


void Camera::Position(float x, float y, float z)
{
	Position(Vector3(x, y, z));
}

void Camera::Position(Vector3& vec)
{
	position = vec;

	Move();
}

void Camera::Position(Vector3* vec)
{
	*vec = position;
}

void Camera::Rotation(float x, float y, float z)
{
	Rotation(Vector3(x, y, z));
}

void Camera::Rotation(Vector3& vec)
{
	rotation = vec;

	Rotation();
}

void Camera::Rotation(Vector3* vec)
{
	*vec = rotation;
}

void Camera::RotationDegree(float x, float y, float z)
{
	RotationDegree(Vector3(x, y, z));
}

void Camera::RotationDegree(Vector3& vec)
{
	rotation = vec * 0.017453f;
}

void Camera::RotationDegree(Vector3* vec)
{
	*vec = rotation * 180 / Math::PI; //TODO. »ó¼öÈ­
}

void Camera::GetMatrix(Matrix* matrix)
{
}

void Camera::Rotation()
{
}

void Camera::Move()
{
}

void Camera::View()
{
}
