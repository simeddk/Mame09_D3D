#pragma once

class SkeletalMeshRenderer
{
public:
	SkeletalMeshRenderer(Shader* shader);
	~SkeletalMeshRenderer();

	void Update();
	void Render();

public:
	void ReadMesh(wstring file);

	Transform* GetTransform() { return transform; }
	SkeletalMesh* GetSkeletalMesh() { return skeletalMesh; }

	void Pass(UINT pass);

private:
	void UpdateTransforms();

private:
	bool bRead = false;

	Shader* shader;
	SkeletalMesh* skeletalMesh;

	Transform* transform; //World

	Matrix transforms[MAX_BONE_COUNT]; //Bone(Local)
};