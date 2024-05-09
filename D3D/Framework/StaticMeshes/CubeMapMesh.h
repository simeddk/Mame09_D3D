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
	Transform* GetTrasnform() { return mesh->GetTransform(); }

private:
	Shader* shader;

	StaticMesh* mesh;

	ID3D11ShaderResourceView* srv = nullptr;
	ID3DX11EffectShaderResourceVariable* sSrv;
};
