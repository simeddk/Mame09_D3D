#include "Framework.h"
#include "SkeletalMesh.h"
#include "Utilities/BinaryFile.h"

SkeletalMesh::SkeletalMesh()
{
}

SkeletalMesh::~SkeletalMesh()
{
    for (SkeletalMesh_Bone* bone : bones)
        SafeDelete(bone);

    for (SkeletalMesh_Mesh* mesh : meshes)
        SafeDelete(mesh);
}

void SkeletalMesh::ReadMesh(wstring file)
{
    file = L"../../_Models/" + file + L".mesh";

    BinaryReader* r = new BinaryReader(file);

    UINT count = 0;

    //Read Bone
    count = r->UInt();
    for (UINT i = 0 ; i < count; i++)
    {
        SkeletalMesh_Bone* bone = new SkeletalMesh_Bone();
        bone->index = r->Int();
        bone->name = String::ToWString(r->String());
        bone->parentIndex = r->Int();
        bone->transform = r->Matrix();

        bones.push_back(bone);
    }

    //Read Mesh
    count = r->UInt();
    for (UINT i = 0; i < count; i++)
    {
        SkeletalMesh_Mesh* mesh = new SkeletalMesh_Mesh();

        mesh->boneIndex = r->Int();

        //Vertices
        {
            UINT count = r->UInt();

            vector<SkeletalMesh::VertexSkeletalMesh> vertices;
            vertices.assign(count, SkeletalMesh::VertexSkeletalMesh());

            void* ptr = (void*)&(vertices[0]);
            r->Byte(&ptr, sizeof(SkeletalMesh::VertexSkeletalMesh) * count);

            mesh->vertices = new SkeletalMesh::VertexSkeletalMesh[count];
            mesh->vertexCount = count;

            copy
            (
                vertices.begin(), vertices.end(),
                stdext::checked_array_iterator<SkeletalMesh::VertexSkeletalMesh*>(mesh->vertices, count)
            );
        }

        //Indices
        {
            UINT count = r->UInt();

            vector<UINT> indices;
            indices.assign(count, UINT());

            void* ptr = (void*)&(indices[0]);
            r->Byte(&ptr, sizeof(UINT) * count);

            mesh->indices = new UINT[count];
            mesh->indexCount = count;

            copy
            (
                indices.begin(), indices.end(),
                stdext::checked_array_iterator<UINT*>(mesh->indices, count)
            );
        }

        //MeshPart
        UINT partCount = r->UInt();
        for (UINT p = 0; p < partCount; p++)
        {
            SkeletalMesh_MeshPart* meshPart = new SkeletalMesh_MeshPart();
            meshPart->materialName = String::ToWString(r->String());

            meshPart->startVertex = r->UInt();
            meshPart->vertexCount = r->UInt();

            meshPart->startIndex = r->UInt();
            meshPart->indexCount = r->UInt();

            mesh->meshParts.push_back(meshPart);
        }

        meshes.push_back(mesh);
    }

    SafeDelete(r);

    BindBone();
    BindMesh();
}

void SkeletalMesh::BindBone()
{
    root = bones[0];

    for (SkeletalMesh_Bone* bone : bones)
    {
        if (bone->parentIndex > -1)
        {
            bone->parent = bones[bone->parentIndex];
            bone->parent->children.push_back(bone);
        }
        else
            bone->parent = nullptr;
    }
}

void SkeletalMesh::BindMesh()
{
    for (SkeletalMesh_Mesh* mesh : meshes)
    {
        mesh->bone = bones[mesh->boneIndex];
        mesh->Binding(this);
    }
}

SkeletalMesh_Bone* SkeletalMesh::BoneByName(wstring index)
{
    for (SkeletalMesh_Bone* bone : bones)
    {
        if (bone->Name() == index)
            return bone;
    }

    return nullptr;
}
