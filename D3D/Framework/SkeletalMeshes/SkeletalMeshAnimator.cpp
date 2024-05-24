#include "Framework.h"
#include "SkeletalMeshAnimator.h"

SkeletalMeshAnimator::SkeletalMeshAnimator(Shader* shader)
	: shader(shader)
{
	skeletalMesh = new SkeletalMesh();
	transform = new Transform(shader);

	frameBuffer = new ConstantBuffer(&keyframeDesc, sizeof(KeyframeDesc));
}

SkeletalMeshAnimator::~SkeletalMeshAnimator()
{
	SafeDelete(skeletalMesh);
	SafeDelete(transform);

	SafeDeleteArray(clipTransforms);

	SafeRelease(texture);
	SafeRelease(transformsSRV);

	SafeDelete(frameBuffer);
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
	frameBuffer->Map();
	sFrameBuffer->SetConstantBuffer(frameBuffer->Buffer());
	
	sTransformsSRV->SetResource(transformsSRV);

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

	sTransformsSRV = shader->AsSRV("TransformsMap");
	sFrameBuffer = shader->AsConstantBuffer("CB_Keyframes");

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

		UINT pageSize = MAX_BONE_COUNT * MAX_KEYFRAME_COUNT * sizeof(Matrix);
		void* p = VirtualAlloc(nullptr, pageSize * skeletalMesh->ClipCount(), MEM_RESERVE, PAGE_READWRITE);

		for (UINT c = 0; c < skeletalMesh->ClipCount(); c++)
		{
			UINT start = c * pageSize;

			for (UINT k = 0; k < MAX_KEYFRAME_COUNT; k++)
			{
				void* temp = (BYTE*)p + MAX_BONE_COUNT * sizeof(Matrix) * k + start;
				VirtualAlloc(temp, MAX_BONE_COUNT * sizeof(Matrix), MEM_COMMIT, PAGE_READWRITE);
				memcpy(temp, clipTransforms[c].Transform[k], MAX_BONE_COUNT * sizeof(Matrix));
			}
		}


		D3D11_SUBRESOURCE_DATA* subResource = new D3D11_SUBRESOURCE_DATA[skeletalMesh->ClipCount()];
		for (UINT c = 0; c < skeletalMesh->ClipCount(); c++)
		{
			void* temp = (BYTE*)p + c * pageSize;

			subResource[c].pSysMem = temp;
			subResource[c].SysMemPitch = MAX_BONE_COUNT * sizeof(Matrix);
			subResource[c].SysMemSlicePitch = pageSize;
		}
		Check(D3D::GetDevice()->CreateTexture2D(&desc, subResource, &texture));

		SafeDeleteArray(subResource);
		VirtualFree(p, 0, MEM_RELEASE);
	}

	//Create SRV
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
		desc.Texture2DArray.MipLevels = 1;
		desc.Texture2DArray.ArraySize = skeletalMesh->ClipCount();

		Check(D3D::GetDevice()->CreateShaderResourceView(texture, &desc, &transformsSRV));
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
