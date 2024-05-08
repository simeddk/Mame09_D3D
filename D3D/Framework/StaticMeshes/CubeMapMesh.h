#pragma once

class CubeMapMesh
{
public:
	CubeMapMesh(Shader* shader);
	~CubeMapMesh();

	void Texture(wstring ddsFile);

	void Update();
	void Render();

public:
	void Position(float x, float y, float z);
	void Position(Vector3& vec);
	void Position(Vector3* vec);

	void Scale(float x, float y, float z);
	void Scale(Vector3& vec);
	void Scale(Vector3* vec);

private:
	Shader* shader;

	StaticMesh* mesh;

	ID3D11ShaderResourceView* srv = nullptr;
	ID3DX11EffectShaderResourceVariable* sSrv;
};
