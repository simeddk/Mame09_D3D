#include "Framework.h"
#include "StaticMesh_Cylinder.h"

StaticMesh_Cylinder::StaticMesh_Cylinder(Shader* shader, float radius, float height, UINT stackCount, UINT sliceCount)
	: StaticMesh(shader)
	, topRadius(radius)
	, bottomRadius(radius)
	, height(height)
	, stackCount(stackCount)
	, sliceCount(sliceCount)
{
}

StaticMesh_Cylinder::StaticMesh_Cylinder(Shader* shader, float topRadius, float bottomRadius, float height, UINT stackCount, UINT sliceCount)
	: StaticMesh(shader)
	, topRadius(topRadius)
	, bottomRadius(bottomRadius)
	, height(height)
	, stackCount(stackCount)
	, sliceCount(sliceCount)
{
}

StaticMesh_Cylinder::~StaticMesh_Cylinder()
{
}

void StaticMesh_Cylinder::Create()
{
	vector<VertexMesh> v;

	float stackHeight = height / (float)stackCount;
	float radiusStep = (topRadius - bottomRadius) / (float)stackCount;

	UINT ringCount = stackCount + 1;
	for (UINT i = 0; i < ringCount; i++)
	{
		float y = -0.5f * height + i * stackHeight;
		float r = bottomRadius + i * radiusStep;
		float theta = 2.0f * Math::PI / (float)sliceCount;

		for (UINT k = 0; k <= sliceCount; k++)
		{
			float c = cosf(k * theta);
			float s = sinf(k * theta);


			VertexMesh vertex;
			vertex.Position = Vector3(r * c, y, r * s);
			vertex.Uv = Vector2((float)k / (float)sliceCount, 1.0f - (float)i / (float)stackCount);


			Vector3 tangent = Vector3(-s, 0.0f, c);

			float dr = bottomRadius - topRadius;
			Vector3 biTangent = Vector3(dr * c, -height, dr * s);

			D3DXVec3Cross(&vertex.Normal, &tangent, &biTangent);
			D3DXVec3Normalize(&vertex.Normal, &vertex.Normal);

			v.push_back(vertex);
		}
	}

	vector<UINT> i;
	UINT ringVertexCount = sliceCount + 1;
	for (UINT y = 0; y < stackCount; y++)
	{
		for (UINT x = 0; x < sliceCount; x++)
		{
			i.push_back(y * ringVertexCount + x);
			i.push_back((y + 1) * ringVertexCount + x);
			i.push_back((y + 1) * ringVertexCount + (x + 1));

			i.push_back(y * ringVertexCount + x);
			i.push_back((y + 1) * ringVertexCount + x + 1);
			i.push_back(y * ringVertexCount + x + 1);
		}
	}


	BuildTopCap(v, i);
	BuildBottomCap(v, i);


	vertices = new VertexMesh[v.size()];
	vertexCount = v.size();
	copy(v.begin(), v.end(), stdext::checked_array_iterator<VertexMesh*>(vertices, vertexCount));

	indices = new UINT[i.size()];
	indexCount = i.size();
	copy(i.begin(), i.end(), stdext::checked_array_iterator<UINT*>(indices, indexCount));
}

void StaticMesh_Cylinder::BuildTopCap(vector<VertexMesh>& vertices, vector<UINT>& indices)
{
	float y = 0.5f * height;
	float theta = 2.0f * Math::PI / (float)sliceCount;

	for (UINT i = 0; i <= sliceCount; i++)
	{
		float x = topRadius * cosf(i * theta);
		float z = topRadius * sinf(i * theta);

		float u = x / height + 0.5f;
		float v = z / height + 0.5f;

		vertices.push_back(VertexMesh(x, y, z, u, v, 0, 1, 0));
	}
	vertices.push_back(VertexMesh(0, y, 0, 0.5f, 0.5f, 0, 1, 0));


	UINT baseIndex = vertices.size() - sliceCount - 2;
	UINT centerIndex = vertices.size() - 1;

	for (UINT i = 0; i < sliceCount; i++)
	{
		indices.push_back(centerIndex);
		indices.push_back(baseIndex + i + 1);
		indices.push_back(baseIndex + i);
	}
}

void StaticMesh_Cylinder::BuildBottomCap(vector<VertexMesh>& vertices, vector<UINT>& indices)
{
	float y = -0.5f * height;
	float theta = 2.0f * Math::PI / (float)sliceCount;

	for (UINT i = 0; i <= sliceCount; i++)
	{
		float x = bottomRadius * cosf(i * theta);
		float z = bottomRadius * sinf(i * theta);

		float u = x / height + 0.5f;
		float v = z / height + 0.5f;

		vertices.push_back(VertexMesh(x, y, z, u, v, 0, -1, 0));
	}
	vertices.push_back(VertexMesh(0, y, 0, 0.5f, 0.5f, 0, -1, 0));


	UINT baseIndex = vertices.size() - sliceCount - 2;
	UINT centerIndex = vertices.size() - 1;

	for (UINT i = 0; i < sliceCount; i++)
	{
		indices.push_back(centerIndex);
		indices.push_back(baseIndex + i);
		indices.push_back(baseIndex + i + 1);
	}
}
