#include "Framework.h"
#include "SkeletalMesh.h"
#include "Utilities/BinaryFile.h"
#include "Utilities/Xml.h"

SkeletalMesh::SkeletalMesh()
{
}

SkeletalMesh::~SkeletalMesh()
{
    for (SkeletalMesh_Bone* bone : bones)
        SafeDelete(bone);

    for (SkeletalMesh_Mesh* mesh : meshes)
        SafeDelete(mesh);

    for (Material* material : materials)
        SafeDelete(material);

    for (SkeletalMeshClip* clip : clips)
        SafeDelete(clip);
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
    
}

void SkeletalMesh::ReadMaterial(wstring file)
{
    file = L"../../_Textures/" + file + L".material";

    Xml::XMLDocument* document = new  Xml::XMLDocument();
    Xml::XMLError error = document->LoadFile(String::ToString(file).c_str());
    assert(error == Xml::XML_SUCCESS);

    Xml::XMLElement* root = document->FirstChildElement(); //<Materials>
    Xml::XMLElement* materialNode = root->FirstChildElement(); //<Material>

    do
    {
        Material* material = new Material();

        Xml::XMLElement* node = nullptr;

        node = materialNode->FirstChildElement(); //<Name>
        material->Name(String::ToWString(node->GetText()));

        wstring directory = Path::GetDirectoryName(file);
        String::Replace(&directory, L"../../_Textures/", L"");

        wstring texture = L"";

        node = node->NextSiblingElement(); //<Diffusefile>
        texture = String::ToWString(node->GetText());
        if (texture.length() > 0)
            material->DiffuseMap(directory + texture);

        node = node->NextSiblingElement(); //<Sepcularfile>
        texture = String::ToWString(node->GetText());
        if (texture.length() > 0)
            material->SpecularMap(directory + texture);

        node = node->NextSiblingElement(); //<NormalFile>
        texture = String::ToWString(node->GetText());
        if (texture.length() > 0)
            material->NormalMap(directory + texture);

        Color color;

        node = node->NextSiblingElement(); //<Ambient>
        color.r =  node->FloatAttribute("R");
        color.g =  node->FloatAttribute("G");
        color.b =  node->FloatAttribute("B");
        color.a =  node->FloatAttribute("A");
        material->Ambient(color);

        node = node->NextSiblingElement(); //<Diffuse>
        color.r = node->FloatAttribute("R");
        color.g = node->FloatAttribute("G");
        color.b = node->FloatAttribute("B");
        color.a = node->FloatAttribute("A");
        material->Diffuse(color);

        node = node->NextSiblingElement(); //<Specular>
        color.r = node->FloatAttribute("R");
        color.g = node->FloatAttribute("G");
        color.b = node->FloatAttribute("B");
        color.a = node->FloatAttribute("A");
        material->Specular(color);

        node = node->NextSiblingElement(); //<Emissive>
        color.r = node->FloatAttribute("R");
        color.g = node->FloatAttribute("G");
        color.b = node->FloatAttribute("B");
        color.a = node->FloatAttribute("A");
        material->Emissive(color);

        materials.push_back(material);

        materialNode = materialNode->NextSiblingElement();
    } while (materialNode != nullptr);


    BindMesh();
}

//Todo. *.clip 파일 읽어오기
void SkeletalMesh::ReadClip(wstring file)
{

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

SkeletalMesh_Bone* SkeletalMesh::BoneByName(wstring name)
{
    for (SkeletalMesh_Bone* bone : bones)
    {
        if (bone->Name() == name)
            return bone;
    }

    return nullptr;
}

Material* SkeletalMesh::MaterialByName(wstring name)
{
    for (Material* material : materials)
    {
        if (material->Name() == name)
            return material;
    }

    return nullptr;
}

SkeletalMeshClip* SkeletalMesh::ClipByName(wstring name)
{
    return nullptr;
}
