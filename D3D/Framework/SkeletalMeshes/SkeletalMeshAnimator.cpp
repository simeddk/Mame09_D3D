#include "Framework.h"
#include "SkeletalMeshAnimator.h"

SkeletalMeshAnimator::SkeletalMeshAnimator(Shader* shader)
	: shader(shader)
{
	skeletalMesh = new SkeletalMesh();
	transform = new Transform(shader);
}

SkeletalMeshAnimator::~SkeletalMeshAnimator()
{
	SafeDelete(skeletalMesh);
	SafeDelete(transform);
}

void SkeletalMeshAnimator::Update()
{
	if (texture == nullptr)
	{
		SetShader(shader, true);
		CreateTexture();
	}

	for (SkeletalMesh_Mesh* mesh : skeletalMesh->Meshes())
		mesh->Update();
}

void SkeletalMeshAnimator::Render()
{
	for (SkeletalMesh_Mesh* mesh : skeletalMesh->Meshes())
	{
		mesh->SetTransform(transform); //World
		mesh->Render();
	}
}

void SkeletalMeshAnimator::ReadMesh(wstring file)
{
	skeletalMesh->ReadMesh(file);
}

void SkeletalMeshAnimator::ReadMaterial(wstring file)
{
	skeletalMesh->ReadMaterial(file);
}

void SkeletalMeshAnimator::ReadClip(wstring file)
{
	skeletalMesh->ReadClip(file);
}

void SkeletalMeshAnimator::SetShader(Shader* shader, bool bCalledByUpdate)
{
	this->shader = shader;

	if (bCalledByUpdate == false)
	{
		SafeDelete(transform);
		transform = new Transform(shader);
	}

	for (SkeletalMesh_Mesh* mesh : skeletalMesh->Meshes())
		mesh->SetShader(shader);
}

void SkeletalMeshAnimator::Pass(UINT pass)
{
	for (SkeletalMesh_Mesh* mesh : skeletalMesh->Meshes())
		mesh->Pass(pass);
}

void SkeletalMeshAnimator::CreateTexture()
{
	clipTransforms = new ClipTransform[skeletalMesh->ClipCount()];
	for (UINT i = 0; i < skeletalMesh->ClipCount(); i++)
		CreateClipTransform(i);

	//Create Texture
	{
		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
		desc.Width = MAX_BONE_COUNT * 4;
		desc.Height = MAX_KEYFRAME_COUNT;
		desc.ArraySize = skeletalMesh->ClipCount();
		desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.MipLevels = 1;
		desc.SampleDesc.Count = 1;

		D3D11_SUBRESOURCE_DATA* subResource = new D3D11_SUBRESOURCE_DATA[skeletalMesh->ClipCount()];
		for (UINT c = 0; c < skeletalMesh->ClipCount(); c++)
		{
			subResource[c].pSysMem = nullptr; //º¸·ù
		}
		D3D::GetDevice()->CreateTexture2D(&desc, subResource, &texture);
	}
}

void SkeletalMeshAnimator::CreateClipTransform(UINT index)
{
	Matrix* bones = new Matrix[MAX_BONE_COUNT];

	SkeletalMeshClip* clip = skeletalMesh->ClipByIndex(index);

	for (UINT f = 0; f < clip->FrameCount(); f++)
	{
		for (UINT b = 0; b < skeletalMesh->BoneCount(); b++)
		{
			SkeletalMesh_Bone* bone = skeletalMesh->BoneByIndex(b);
			Matrix inv = bone->Transform();

			D3DXMatrixInverse(&inv, nullptr, &inv);

			Matrix parent;
			int parentIndex = bone->ParentIndeX();
			if (parentIndex < 0)
				D3DXMatrixIdentity(&parent);
			else
				parent = bones[parentIndex];

			Matrix animation;

			SkeletalMeshKeyframe* frame = clip->Keyframe(bone->Name());

			if (frame != nullptr)
			{
				SkeletalMeshKeyframeData& data = frame->Transforms[f];

				Matrix S, R, T;
				D3DXMatrixScaling(&S, data.Scale.x, data.Scale.y, data.Scale.z);
				D3DXMatrixRotationQuaternion(&R, &data.Rotation);
				D3DXMatrixTranslation(&T, data.Translation.x, data.Translation.y, data.Translation.z);

				animation = S * R * T;
			}
			else
			{
				D3DXMatrixIdentity(&animation);
			}

			bones[b] = animation * parent;
			clipTransforms[index].Transform[f][b] = inv * bones[b];;
			
			//(Inv-meshBone * animationBone) * parent
		}
	}
}
