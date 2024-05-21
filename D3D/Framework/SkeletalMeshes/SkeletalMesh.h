#pragma once

#define MAX_BONE_COUNT 250

class SkeletalMesh_Bone;
class SkeletalMesh_Mesh;
class SkeletalMesh_MeshPart;

class SkeletalMesh
{
public:
	friend class SkeletalMeshRenderer;
	struct VertexSkeletalMesh;

private:
	SkeletalMesh();
	~SkeletalMesh();

private:
	void ReadMesh(wstring file);
	void ReadMaterial(wstring file);

private:
	void BindBone();
	void BindMesh();

public:
	UINT BoneCount() { return bones.size(); }
	vector<SkeletalMesh_Bone*>& Bones() { return bones; }
	SkeletalMesh_Bone* BoneByIndex(UINT index) { return bones[index]; }
	SkeletalMesh_Bone* BoneByName(wstring name);

	UINT MeshCount() { return meshes.size(); }
	vector<SkeletalMesh_Mesh*>& Meshes() { return meshes; }
	SkeletalMesh_Mesh* MeshByIndex(UINT index) { return meshes[index]; }

	UINT MaterialCount() { return materials.size(); }
	vector<Material*>& Materials() { return materials; }
	Material* MaterialByIndex(UINT index) { return materials[index]; }
	Material* MaterialByName(wstring name);

public:
	struct VertexSkeletalMesh
	{
		Vector3 Position;
		Vector2 Uv;
		Vector3 Normal;
		Vector3 Tangent;
		Vector4 BlendIndices;
		Vector4 BlendWeights;

		VertexSkeletalMesh()
		{
			Position = Vector3(0, 0, 0);
			Uv = Vector2(0, 0);
			Normal = Vector3(0, 0, 0);
			Tangent = Vector3(0, 0, 0);
			BlendIndices = Vector4(0, 0, 0, 0);
			BlendWeights = Vector4(0, 0, 0, 0);
		}
	};

private:
	SkeletalMesh_Bone* root;

	vector<SkeletalMesh_Bone*> bones;
	vector<SkeletalMesh_Mesh*> meshes;
	vector<Material*> materials;
};