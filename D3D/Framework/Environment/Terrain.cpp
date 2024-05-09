#include "Framework.h"
#include "Terrain.h"

Terrain::Terrain(Shader* shader, wstring heightMapPath)
	: StaticMeshRenderer(shader)
{
	heightMap = new Texture(heightMapPath);

	CreateVertexData();
	CreateIndexData();
	CreateNormalData();
	
	vertexBuffer = new VertexBuffer(vertices, vertexCount, sizeof(VertexTerrain));
	indexBuffer = new IndexBuffer(indices, indexCount);
}

Terrain::~Terrain()
{
	SafeDeleteArray(vertices);
	SafeDeleteArray(indices);

	SafeDelete(heightMap);
}

void Terrain::Update()
{
	static Vector3 lightDirection = Vector3(-1, -1, 1);
	ImGui::SliderFloat3("Light Direction", lightDirection, -1, 1);
	shader->AsVector("LightDirection")->SetFloatVector(lightDirection);

	Super::Update();
}

void Terrain::Render()
{
	visibleNormal();

	if (baseMap != nullptr)
		shader->AsSRV("BaseMap")->SetResource(baseMap->SRV());

	if (layerMap != nullptr)
		shader->AsSRV("LayerMap")->SetResource(layerMap->SRV());

	if (alphaMap != nullptr)
		shader->AsSRV("AlphaMap")->SetResource(alphaMap->SRV());

	shader->AsScalar("Tile")->SetFloat(tile);
	shader->AsScalar("Intensity")->SetFloat(intensity);

	Super::Render();

	shader->DrawIndexed(0, Pass(), indexCount);
}

float Terrain::GetHeightByInterp(Vector3 position)
{
	UINT x = (UINT)position.x;
	UINT z = (UINT)position.z;

	if (x < 0 || x > width - 2) return FLT_MIN;
	if (z < 0 || z > height - 2) return FLT_MIN;

	UINT index[4];
	index[0] = width * z + x;
	index[1] = width * (z + 1) + x;
	index[2] = width * z + (x + 1);
	index[3] = width * (z + 1) + (x + 1);
	
	Vector3 p[4];
	for (UINT i = 0 ; i < 4; i++)
		p[i] = vertices[index[i]].Position;

	float ddx = position.x - p[0].x;
	float ddz = position.z - p[0].z;

	Vector3 result;
	if (ddx + ddz <= 1)
	{
		result = p[0] + (p[2] - p[0]) * ddx + (p[1] - p[0]) * ddz;
	}
	else
	{
		ddx = 1.f - ddx;
		ddz = 1.f - ddz;

		result = p[3] + (p[1] - p[3]) * ddx + (p[2] - p[3]) * ddz;
	}

	return result.y;
}

float Terrain::GetHeightByRaycast(Vector3 position)
{
	UINT x = (UINT)position.x;
	UINT z = (UINT)position.z;

	if (x < 0 || x > width - 2) return FLT_MIN;
	if (z < 0 || z > height - 2) return FLT_MIN;

	UINT index[4];
	index[0] = width * z + x;
	index[1] = width * (z + 1) + x;
	index[2] = width * z + (x + 1);
	index[3] = width * (z + 1) + (x + 1);

	Vector3 p[4];
	for (UINT i = 0; i < 4; i++)
		p[i] = vertices[index[i]].Position;

	Vector3 result(-1, FLT_MIN, -1);

	Vector3 start(position.x, 100.f, position.z);
	Vector3 direction(0, -1, 0);

	float u, v, distance;
	if (D3DXIntersectTri(&p[0], &p[1], &p[2], &start, &direction, &u, &v, &distance))
		result = p[0] + (p[1] - p[0]) * u + (p[2] - p[0]) * v;

	if (D3DXIntersectTri(&p[3], &p[1], &p[2], &start, &direction, &u, &v, &distance))
		result = p[3] + (p[1] - p[3]) * u + (p[2] - p[3]) * v;

	return result.y;
}

void Terrain::visibleNormal()
{
	//Draw Debug Normal
	ImGui::Checkbox("Visible Normal", &bVisibleNormal);
	if (bVisibleNormal)
	{
		for (UINT z = 0; z < height; z += vertexInterval)
		{
			for (UINT x = 0; x < width; x += vertexInterval)
			{
				UINT index = width * z + x;

				Vector3 start = vertices[index].Position;
				Vector3 end = start + vertices[index].Normal;

				DebugLine::Get()->RenderLine(start, end, Color(1, 0, 0, 1));
			}
		}
	}
}

void Terrain::BaseMap(wstring file)
{
	SafeDelete(baseMap);

	baseMap = new Texture(file);
}

void Terrain::LayerMap(wstring file)
{
	SafeDelete(layerMap);

	layerMap = new Texture(file);
}

void Terrain::AlphaMap(wstring file)
{
	SafeDelete(alphaMap);

	alphaMap = new Texture(file);
}

void Terrain::CreateVertexData()
{
	width = heightMap->GetWidth();
	height = heightMap->GetHeight();
	
	vector<Color> pixels;
	heightMap->ReadPixel(&pixels);

	vertexCount = width * height;
	vertices = new VertexTerrain[vertexCount];

	for (UINT y = 0; y < height; y++)
	{
		for (UINT x = 0; x < width; x++)
		{
			UINT index = width * y + x;
			UINT reverse = width * (height - y - 1) + x;

			vertices[index].Position.x = (float)x;
			vertices[index].Position.y = pixels[reverse].r * 255.f / 10.f;
			vertices[index].Position.z = (float)y;

			vertices[index].Uv.x = x / ((float)width - 1);
			vertices[index].Uv.y = 1 - (y / ((float)height - 1));
		}
	}
}

void Terrain::CreateIndexData()
{
	indexCount = (width - 1) * (height - 1) * 6;
	indices = new UINT[indexCount];

	UINT index = 0;
	for (UINT y = 0; y < height - 1; y++)
	{
		for (UINT x = 0; x < width - 1; x++)
		{
			indices[index + 0] = width * y + x;
			indices[index + 1] = width * (y + 1) + x;
			indices[index + 2] = width * y + (x + 1);
			indices[index + 3] = width * y + (x + 1);
			indices[index + 4] = width * (y + 1) + x;
			indices[index + 5] = width * (y + 1) + (x + 1);

			index += 6;
		}
	}
}

void Terrain::CreateNormalData()
{
	for(UINT i = 0 ; i < indexCount / 3 ; i++)
	{
		UINT index0 = indices[i * 3 + 0];
		UINT index1 = indices[i * 3 + 1];
		UINT index2 = indices[i * 3 + 2];

		VertexTerrain v0 = vertices[index0];
		VertexTerrain v1 = vertices[index1];
		VertexTerrain v2 = vertices[index2];

		Vector3 e1 = v1.Position - v0.Position;
		Vector3 e2 = v2.Position - v0.Position;

		Vector3 normal;
		D3DXVec3Cross(&normal, &e1, &e2);
		D3DXVec3Normalize(&normal, &normal);

		vertices[index0].Normal += normal;
		vertices[index1].Normal += normal;
		vertices[index2].Normal += normal;
	}


	for (UINT i = 0; i < vertexCount; i++)
		D3DXVec3Normalize(&vertices[i].Normal, &vertices[i].Normal);
}