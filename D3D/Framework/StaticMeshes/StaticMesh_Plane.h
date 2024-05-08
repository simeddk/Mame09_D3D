#pragma once

class StaticMesh_Plane : public StaticMesh
{
public:
	StaticMesh_Plane(Shader* shader, float tileU = 1.f, float tileV = 1.f);
	~StaticMesh_Plane();

private:
	void Create() override;

private:
	float tileU;
	float tileV;
};