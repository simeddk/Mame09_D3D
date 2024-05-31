#include "Framework.h"
#include "SkeletalMeshAnimator.h"

SkeletalMeshAnimator::SkeletalMeshAnimator(Shader* shader)
	: shader(shader)
{
	skeletalMesh = new SkeletalMesh();
	transform = new Transform(shader);

	frameBuffer = new ConstantBuffer(&tweenDesc, sizeof(TweenDesc));
	blendBuffer = new ConstantBuffer(&blendDesc, sizeof(BlendDesc));

	//Create ComputeShader
	{

		computeShader = new Shader(L"21_GetBones.fxo");


		sComputeWorld = computeShader->AsMatrix("World");

		sComputeFrameBuffer = computeShader->AsConstantBuffer("CB_Animationframe");
		sComputeBlendBuffer = computeShader->AsConstantBuffer("CB_Blendingframe");
		sComputeTransformsSRV = computeShader->AsSRV("TransformsMap");

		computeBoneBuffer = new StructuredBuffer(nullptr, sizeof(Matrix), MAX_BONE_COUNT, sizeof(Matrix), MAX_BONE_COUNT);
		sComputeInputBoneBuffer = computeShader->AsSRV("InputBones");
		sComputeOutputBoneBuffer = computeShader->AsUAV("OutputBones");
	}
}

SkeletalMeshAnimator::~SkeletalMeshAnimator()
{
	SafeDelete(skeletalMesh);
	SafeDelete(transform);

	SafeDeleteArray(clipTransforms);

	SafeRelease(texture);
	SafeRelease(transformsSRV);

	SafeDelete(frameBuffer);
	SafeDelete(blendBuffer);

	SafeDelete(computeShader);
	SafeDelete(computeBoneBuffer);
}

void SkeletalMeshAnimator::Update()
{
	if (texture == nullptr)
	{
		SetShader(shader, true);
		CreateTexture();

		Matrix bones[MAX_BONE_COUNT];
		for (UINT i = 0; i < skeletalMesh->BoneCount(); i++)
			bones[i] = skeletalMesh->BoneByIndex(i)->Transform();

		computeBoneBuffer->CopyToInput(bones);
	}

	if (blendDesc.Mode == 0)
		UpdateAnimationFrame();
	else
		UpdateBlendingFrame();



	frameBuffer->Map();
	blendBuffer->Map();

	frameTime += Time::Delta();
	if (frameTime > (1.0f / frameRate))
	{
		sComputeWorld->SetMatrix(transform->World());

		sComputeFrameBuffer->SetConstantBuffer(frameBuffer->Buffer());
		sComputeBlendBuffer->SetConstantBuffer(blendBuffer->Buffer());
		sComputeTransformsSRV->SetResource(transformsSRV);

		sComputeInputBoneBuffer->SetResource(computeBoneBuffer->SRV());
		sComputeOutputBoneBuffer->SetUnorderedAccessView(computeBoneBuffer->UAV());

		computeShader->Dispatch(0, 0, 1, 1, 1);
	}

	frameTime = fmod(frameTime, (1.0f / frameRate));



	for (SkeletalMesh_Mesh* mesh : skeletalMesh->Meshes())
		mesh->Update();
}

void SkeletalMeshAnimator::Render()
{
	sFrameBuffer->SetConstantBuffer(frameBuffer->Buffer());
	sBlendBuffer->SetConstantBuffer(blendBuffer->Buffer());
	
	sTransformsSRV->SetResource(transformsSRV);

	for (SkeletalMesh_Mesh* mesh : skeletalMesh->Meshes())
	{
		mesh->SetTransform(transform); //World
		mesh->Render();
	}
}

void SkeletalMeshAnimator::UpdateAnimationFrame()
{
	TweenDesc& desc = tweenDesc;
	SkeletalMeshClip* clip = skeletalMesh->ClipByIndex(desc.Curr.Clip);

	//Current Clip
	desc.Curr.RunningTime += Time::Delta();

	float ratio = 1.f / clip->FrameRate() / desc.Curr.Speed;
	if (desc.Curr.Time >= 1.f)
	{
		desc.Curr.RunningTime = 0.f;

		desc.Curr.CurrFrame = (desc.Curr.CurrFrame + 1) % clip->FrameCount();
		desc.Curr.NextFrame = (desc.Curr.CurrFrame + 1) % clip->FrameCount();
	}
	desc.Curr.Time = desc.Curr.RunningTime / ratio;

	//Next Clip
	if (desc.Next.Clip > -1)
	{
		SkeletalMeshClip* nextClip = skeletalMesh->ClipByIndex(desc.Next.Clip);

		desc.ChangeTime += Time::Delta();
		desc.TweenTime = desc.ChangeTime / desc.TakeTime;

		if (desc.TweenTime >= 1.f)
		{
			desc.Curr = desc.Next;

			desc.Next.Clip = -1;
			desc.Next.CurrFrame = 0;
			desc.Next.NextFrame = 0;
			desc.Next.Time = 0;
			desc.Next.RunningTime = 0;

			desc.ChangeTime = 0;
			desc.TweenTime = 0;

			desc.Next.Speed = desc.Curr.Speed;
		}
		else
		{
			desc.Next.RunningTime += Time::Delta();

			float ratio = 1.f / nextClip->FrameRate() / desc.Next.Speed;
			if (desc.Next.Time >= 1.f)
			{
				desc.Next.RunningTime = 0.f;

				desc.Next.CurrFrame = (desc.Next.CurrFrame + 1) % nextClip->FrameCount();
				desc.Next.NextFrame = (desc.Next.CurrFrame + 1) % nextClip->FrameCount();
			}
			desc.Next.Time = desc.Next.RunningTime / ratio;
		}
		
	}
	
}

void SkeletalMeshAnimator::UpdateBlendingFrame()
{
	BlendDesc& desc = blendDesc;
	
	for (UINT i = 0; i < 3; i++)
	{
		SkeletalMeshClip* clip = skeletalMesh->ClipByIndex(desc.Clips[i].Clip);
		desc.Clips[i].RunningTime += Time::Delta();

		float ratio = 1.f / clip->FrameRate() / desc.Clips[i].Speed;
		if (desc.Clips[i].Time >= 1.f)
		{
			desc.Clips[i].RunningTime = 0.f;

			desc.Clips[i].CurrFrame = (desc.Clips[i].CurrFrame + 1) % clip->FrameCount();
			desc.Clips[i].NextFrame = (desc.Clips[i].CurrFrame + 1) % clip->FrameCount();
		}
		desc.Clips[i].Time = desc.Clips[i].RunningTime / ratio;
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

void SkeletalMeshAnimator::PlayTweenMode(UINT clip, float speed, float taketime)
{
	blendDesc.Mode = 0;

	tweenDesc.TakeTime = taketime;
	tweenDesc.Next.Clip = clip;
	tweenDesc.Next.Speed = speed;
}

void SkeletalMeshAnimator::PlayBlendMode(UINT clip1, UINT clip2, UINT clip3)
{
	blendDesc.Mode = 1;

	blendDesc.Clips[0].Clip = clip1;
	blendDesc.Clips[1].Clip = clip2;
	blendDesc.Clips[2].Clip = clip3;
}

void SkeletalMeshAnimator::SetBlendAlpha(float alpha)
{
	alpha =  Math::Clamp(alpha, 0.f, 2.f);

	blendDesc.Alpha = alpha;
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
	sFrameBuffer = shader->AsConstantBuffer("CB_Animationframe");
	sBlendBuffer = shader->AsConstantBuffer("CB_Blendingframe");

	for (SkeletalMesh_Mesh* mesh : skeletalMesh->Meshes())
		mesh->SetShader(shader);
}

void SkeletalMeshAnimator::Pass(UINT pass)
{
	for (SkeletalMesh_Mesh* mesh : skeletalMesh->Meshes())
		mesh->Pass(pass);
}

void SkeletalMeshAnimator::GetAttachBones(Matrix* matrix)
{
	computeBoneBuffer->CopyFromOutput(matrix);
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
