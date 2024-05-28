#pragma once

class SkeletalMeshAnimator
{
public:
	SkeletalMeshAnimator(Shader* shader);
	~SkeletalMeshAnimator();

	void Update();
	void Render();

private:
	void UpdateAnimationFrame();
	void UpdateBlendingFrame();

public:
	void ReadMesh(wstring file);
	void ReadMaterial(wstring file);
	void ReadClip(wstring file);

	void PlayTweenMode(UINT clip, float speed = 1.f, float taketime = 0.25f);
	void PlayBlendMode(UINT clip1, UINT clip2, UINT clip3);
	void SetBlendAlpha(float alpha);

public:
	void SetShader(Shader* shader, bool bCalledByUpdate = false);
	void Pass(UINT pass);

	Transform* GetTransform() { return transform; }
	SkeletalMesh* GetSkeletalMesh() { return skeletalMesh; }

private:
	void CreateTexture();
	void CreateClipTransform(UINT index);

private:
	struct ClipTransform
	{
		Matrix** Transform;

		ClipTransform()
		{
			Transform = new Matrix*[MAX_KEYFRAME_COUNT];

			for (UINT i = 0; i < MAX_KEYFRAME_COUNT; i++)
				Transform[i] = new Matrix[MAX_BONE_COUNT];
		}

		~ClipTransform()
		{
			for (UINT i = 0; i < MAX_KEYFRAME_COUNT; i++)
				SafeDeleteArray(Transform[i]);

			SafeDeleteArray(Transform);
		}

	};
	ClipTransform* clipTransforms;


private:
	ID3D11Texture2D* texture = nullptr;
	ID3D11ShaderResourceView* transformsSRV;
	ID3DX11EffectShaderResourceVariable* sTransformsSRV;

private:
	struct KeyframeDesc
	{
		int Clip = 0;

		UINT CurrFrame = 0;
		UINT NextFrame = 0;

		float Time = 0.f;
		float RunningTime = 0.f;

		float Speed = 1.f;

		Vector2 Padding;
	};

private:
	struct TweenDesc
	{
		float TakeTime = 0.25f; //Requirement Time
		float TweenTime = 0.f; //Ratio
		float ChangeTime = 0.f; //Running Time
		float Padding;

		KeyframeDesc Curr;
		KeyframeDesc Next;

		TweenDesc()
		{
			Curr.Clip = 0;
			Next.Clip = -1;
		}
	} tweenDesc;

	ConstantBuffer* frameBuffer;
	ID3DX11EffectConstantBuffer* sFrameBuffer;

private:
	struct BlendDesc
	{
		UINT Mode = 0;
		float Alpha = 0;
		Vector2 Padding;

		KeyframeDesc Clips[3];
	} blendDesc;

	ConstantBuffer* blendBuffer;
	ID3DX11EffectConstantBuffer* sBlendBuffer;

private:
	Shader* shader;
	SkeletalMesh* skeletalMesh;

	Transform* transform; //World
};