#include "Framework.h"
#include "StaticMesh_Quad.h"

StaticMesh_Quad::StaticMesh_Quad(Shader* shader)
	: StaticMesh(shader)
{
}

StaticMesh_Quad::~StaticMesh_Quad()
{
}

void StaticMesh_Quad::Create()
{
	float w = 0.5f;
	float h = 0.5f;

	vector<VertexMesh> v;
	v.push_back(VertexMesh(-w, -h, 0, 0, 1, 0, 0, -1));
	v.push_back(VertexMesh(-w, +h, 0, 0, 0, 0, 0, -1));
	v.push_back(VertexMesh(+w, -h, 0, 1, 1, 0, 0, -1));
	v.push_back(VertexMesh(+w, +h, 0, 1, 0, 0, 0, -1));

	vertexCount = 4;
	vertices = new VertexMesh[vertexCount];

	copy(v.begin(), v.end(), stdext::checked_array_iterator<VertexMesh*>(vertices, vertexCount));

	indexCount = 6;
	indices = new UINT[indexCount]{ 0, 1, 2, 2, 1, 3 };
}
