#pragma once

class Sky
{
public:
	Sky(wstring ddsFile);
	~Sky();

	void Update();
	void Render();

public:
	Shader* GetShader() { return shader; }
	void Pass(UINT val) { sphere->Pass(val); }

private:
	Shader* shader;
	StaticMesh_Sphere* sphere;

	ID3D11ShaderResourceView* srv;
	ID3DX11EffectShaderResourceVariable* sSrv;
};