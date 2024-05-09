#pragma once

class Terrain : public StaticMeshRenderer
{
public:
	Terrain(Shader* shader, wstring heightMapPath);
	~Terrain();

	void Update();
	void Render();

	float GetHeightByInterp(Vector3 position);
	float GetHeightByRaycast(Vector3 position);

	void visibleNormal();

	void BaseMap(wstring file);
	void LayerMap(wstring file);
	void AlphaMap(wstring file);

	void Tile(float tile) { this->tile = tile; }
	void Intensity(float intensity) { this->intensity = intensity; }

private:
	void CreateVertexData();
	void CreateIndexData();
	void CreateNormalData();


private:
	struct VertexTerrain
	{
		Vector3 Position = Vector3(0, 0, 0);
		Vector3 Normal = Vector3(0, 0, 0);
		Vector2 Uv = Vector2(0, 0);
	};


private:
	UINT width, height;

	VertexTerrain* vertices;
	UINT* indices;

	Texture* heightMap;
	Texture* baseMap = nullptr;
	Texture* layerMap = nullptr;
	Texture* alphaMap = nullptr;

	float tile = 4.f;
	float intensity = 2.5f;

	bool bVisibleNormal = false;
	UINT vertexInterval = 3;
};