#pragma once

//-----------------------------------------------------------------------------
//SkeletalBone
//-----------------------------------------------------------------------------
class SkeletalMesh_Bone
{
public:
	friend class SkeletalMesh;

private:
	SkeletalMesh_Bone() = default;
	~SkeletalMesh_Bone() = default;

public:
	int Index() { return index; }

	wstring Name() { return name; }

	int ParentIndeX() { return parentIndex; }
	SkeletalMesh_Bone* Parent() { return parent; }

	Matrix& Transform() { return transform; }
	void Transform(Matrix& matrix) { transform = matrix; }

	vector<SkeletalMesh_Bone*>& Children() { return children; }

private:
	int index;
	wstring name;

	int parentIndex;
	SkeletalMesh_Bone* parent;

	Matrix transform;
	vector< SkeletalMesh_Bone*> children;
};

//-----------------------------------------------------------------------------
//SkeletalMesh_Mesh
//-----------------------------------------------------------------------------
class SkeletalMesh_Mesh
{
public:
	friend class SkeletalMesh;

private:
	SkeletalMesh_Mesh();
	~SkeletalMesh_Mesh();

	void Binding(SkeletalMesh* skeletalMesh);

public:
	void Pass(UINT val);

	void SetShader(Shader* shader);

	void Update();
	void Render();

	int BoneIndex() { return boneIndex; }
	SkeletalMesh_Bone* Bone() { return bone; }

	void Transforms(Matrix* transforms); //Component(Bone) Space
	void SetTransform(Transform* transform); //Actor(World)Transform

private:
	struct BoneDesc
	{
		Matrix Transforms[MAX_BONE_COUNT]; //Component(Bone) Space

		UINT BoneIndex;
		float Padding[3];
	} boneDesc;

private:
	Shader* shader;

	Transform* transform = nullptr; //Actor(World)Transform
	PerFrame* perFrame = nullptr;

	int boneIndex;
	SkeletalMesh_Bone* bone;

	VertexBuffer* vertexBuffer;
	UINT vertexCount;
	SkeletalMesh::VertexSkeletalMesh* vertices;

	IndexBuffer* indexBuffer;
	UINT indexCount;
	UINT* indices;

	ConstantBuffer* boneBuffer;
	ID3DX11EffectConstantBuffer* sBoneBuffer;

	vector<class SkeletalMesh_MeshPart*> meshParts;
};

//-----------------------------------------------------------------------------
//SkeletalMesh_MeshPart
//-----------------------------------------------------------------------------
class SkeletalMesh_MeshPart
{
public:
	friend class SkeletalMesh;
	friend class SkeletalMesh_Mesh;

private:
	SkeletalMesh_MeshPart() = default;
	~SkeletalMesh_MeshPart() = default;

	void Update();
	void Render();

	void Binding(SkeletalMesh* skeletalMesh);
	void SetShader(Shader* shader);

	void Pass(UINT val) { pass = val; }

private:
	Shader* shader;
	UINT pass = 0;

	wstring materialName;

	UINT startVertex;
	UINT vertexCount;

	UINT startIndex;
	UINT indexCount;
};