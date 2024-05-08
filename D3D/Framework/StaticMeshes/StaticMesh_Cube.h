#pragma once

class StaticMesh_Cube : public StaticMesh
{
public:
	StaticMesh_Cube(Shader* shader);
	~StaticMesh_Cube();

private:
	void Create() override;

};