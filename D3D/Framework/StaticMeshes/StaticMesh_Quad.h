#pragma once

class StaticMesh_Quad : public StaticMesh
{
public:
	StaticMesh_Quad(Shader* shader);
	~StaticMesh_Quad();

private:
	void Create() override;
};