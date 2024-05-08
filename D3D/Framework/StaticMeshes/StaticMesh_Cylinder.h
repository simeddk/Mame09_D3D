#pragma once

class StaticMesh_Cylinder : public StaticMesh
{
public:
	StaticMesh_Cylinder(Shader* shader, float radius, float height, UINT stackCount, UINT sliceCount);
	StaticMesh_Cylinder(Shader* shader, float topRadius, float bottomRadius, float height, UINT stackCount, UINT sliceCount);
	~StaticMesh_Cylinder();

private:
	void Create() override;

private:
	void BuildTopCap(vector<VertexMesh>& vertices, vector<UINT>& indices);
	void BuildBottomCap(vector<VertexMesh>& vertices, vector<UINT>& indices);

private:
	float topRadius;
	float bottomRadius;
	float height;


	UINT stackCount;
	UINT sliceCount;

};
