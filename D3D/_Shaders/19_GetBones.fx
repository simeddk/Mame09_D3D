#include "00_Global.fx"

struct BoneDesc
{
	matrix Transform;
};
StructuredBuffer<BoneDesc> InputBones;
RWStructuredBuffer<BoneDesc> OutputBones;

#define MAX_BONE_COUNT 250

struct KeyframeDesc
{
	int Clip;

	uint CurrFrame;
	uint NextFrame;

	float Time;
	float RunningTime;

	float Speed;
};

struct TweenFrame
{
	float TakeTime;
	float TweenTime;
	float ChangeTime;
	float Padding;

	KeyframeDesc Curr;
	KeyframeDesc Next;
};

cbuffer CB_Animationframe
{
	TweenFrame TweenFrames;
};

Texture2DArray TransformsMap;

//-----------------------------------------------------------------------------
//SetAnimationWorld (TweenMode)
//-----------------------------------------------------------------------------
void SetAnimationWorld(inout matrix world, uint3 id)
{
	//[0]currClip, [1]nextClip
	int clip[2];
	int currFrame[2], nextFrame[2];
	float time[2];
	
	//currentClip
	clip[0] = TweenFrames.Curr.Clip;
	currFrame[0] = TweenFrames.Curr.CurrFrame;
	nextFrame[0] = TweenFrames.Curr.NextFrame;
	time[0] = TweenFrames.Curr.Time;
	
	//nextClip
	clip[1] = TweenFrames.Next.Clip;
	currFrame[1] = TweenFrames.Next.CurrFrame;
	nextFrame[1] = TweenFrames.Next.NextFrame;
	time[1] = TweenFrames.Next.Time;
	
	float4 c0, c1, c2, c3;
	float4 n0, n1, n2, n3;
	
	matrix transform = 0;
	matrix curr = 0, next = 0;
	matrix currAnim = 0, nextAnim = 0;
	
	//-----------------------------------------------------------------
	//Current Clip
	//-----------------------------------------------------------------
	//Current Frame
	c0 = TransformsMap.Load(int4(id.x * 4 + 0, currFrame[0], clip[0], 0)); //_11_12_13_14
	c1 = TransformsMap.Load(int4(id.x * 4 + 1, currFrame[0], clip[0], 0)); //_21_22_23_24
	c2 = TransformsMap.Load(int4(id.x * 4 + 2, currFrame[0], clip[0], 0)); //_31_32_33_34
	c3 = TransformsMap.Load(int4(id.x * 4 + 3, currFrame[0], clip[0], 0)); //_41_42_43_44
	curr = matrix(c0, c1, c2, c3);
	
	//Next Frame
	n0 = TransformsMap.Load(int4(id.x * 4 + 0, nextFrame[0], clip[0], 0)); //_11_12_13_14
	n1 = TransformsMap.Load(int4(id.x * 4 + 1, nextFrame[0], clip[0], 0)); //_21_22_23_24
	n2 = TransformsMap.Load(int4(id.x * 4 + 2, nextFrame[0], clip[0], 0)); //_31_32_33_34
	n3 = TransformsMap.Load(int4(id.x * 4 + 3, nextFrame[0], clip[0], 0)); //_41_42_43_44
	next = matrix(n0, n1, n2, n3);
	
	//Inter Frame Lerp
	currAnim = lerp(curr, next, time[0]);
	
	//-----------------------------------------------------------------
	//Next Clip
	//-----------------------------------------------------------------
	[flatten]
	if (clip[1] > -1)
	{
		//Current Frame
		c0 = TransformsMap.Load(int4(id.x * 4 + 0, currFrame[1], clip[1], 0)); //_11_12_13_14
		c1 = TransformsMap.Load(int4(id.x * 4 + 1, currFrame[1], clip[1], 0)); //_21_22_23_24
		c2 = TransformsMap.Load(int4(id.x * 4 + 2, currFrame[1], clip[1], 0)); //_31_32_33_34
		c3 = TransformsMap.Load(int4(id.x * 4 + 3, currFrame[1], clip[1], 0)); //_41_42_43_44
		curr = matrix(c0, c1, c2, c3);
	
		//Next Frame
		n0 = TransformsMap.Load(int4(id.x * 4 + 0, nextFrame[1], clip[1], 0)); //_11_12_13_14
		n1 = TransformsMap.Load(int4(id.x * 4 + 1, nextFrame[1], clip[1], 0)); //_21_22_23_24
		n2 = TransformsMap.Load(int4(id.x * 4 + 2, nextFrame[1], clip[1], 0)); //_31_32_33_34
		n3 = TransformsMap.Load(int4(id.x * 4 + 3, nextFrame[1], clip[1], 0)); //_41_42_43_44
		next = matrix(n0, n1, n2, n3);
	
		//Inter Frame Lerp
		nextAnim = lerp(curr, next, time[1]);
			
		//Inter Clip Lerp
		currAnim = lerp(currAnim, nextAnim, TweenFrames.TweenTime);
	}
	
	world = mul(currAnim, world);
}

//-----------------------------------------------------------------------------
//SetBlendginWorld (BlendMode)
//-----------------------------------------------------------------------------
struct BlendDesc
{
	uint Mode;
	float Alpha;
	float2 Padding;

	KeyframeDesc Clips[3];
};

cbuffer CB_Blendingframe
{
	BlendDesc BlendingFrames;
};

void SetBlendingWorld(inout matrix world, uint3 id)
{
	float4 c0, c1, c2, c3;
	float4 n0, n1, n2, n3;
	
	matrix transform = 0;
	matrix curr = 0, next = 0;
	matrix currAnim[3];
	matrix anim;
	
	
	[unroll(3)]
	for (int j = 0; j < 3; j++)
	{
		//Current Frame
		c0 = TransformsMap.Load(int4(id.x * 4 + 0, BlendingFrames.Clips[j].CurrFrame, BlendingFrames.Clips[j].Clip, 0)); //_11_12_13_14
		c1 = TransformsMap.Load(int4(id.x * 4 + 1, BlendingFrames.Clips[j].CurrFrame, BlendingFrames.Clips[j].Clip, 0)); //_21_22_23_24
		c2 = TransformsMap.Load(int4(id.x * 4 + 2, BlendingFrames.Clips[j].CurrFrame, BlendingFrames.Clips[j].Clip, 0)); //_31_32_33_34
		c3 = TransformsMap.Load(int4(id.x * 4 + 3, BlendingFrames.Clips[j].CurrFrame, BlendingFrames.Clips[j].Clip, 0)); //_41_42_43_44
		curr = matrix(c0, c1, c2, c3);
	
		//Next Frame
		n0 = TransformsMap.Load(int4(id.x * 4 + 0, BlendingFrames.Clips[j].NextFrame, BlendingFrames.Clips[j].Clip, 0)); //_11_12_13_14
		n1 = TransformsMap.Load(int4(id.x * 4 + 1, BlendingFrames.Clips[j].NextFrame, BlendingFrames.Clips[j].Clip, 0)); //_21_22_23_24
		n2 = TransformsMap.Load(int4(id.x * 4 + 2, BlendingFrames.Clips[j].NextFrame, BlendingFrames.Clips[j].Clip, 0)); //_31_32_33_34
		n3 = TransformsMap.Load(int4(id.x * 4 + 3, BlendingFrames.Clips[j].NextFrame, BlendingFrames.Clips[j].Clip, 0)); //_41_42_43_44
		next = matrix(n0, n1, n2, n3);
	
		//Inter Frame Lerp
		currAnim[j] = lerp(curr, next, BlendingFrames.Clips[j].Time);
	}
	
	float alpha = BlendingFrames.Alpha;
	int clipIndex[2] = { 0, 1 };
	if (alpha > 1)
	{
		clipIndex[0] = 1;
		clipIndex[1] = 2;

		alpha -= 1.f;
	}
	
	//Inter Clip Lerp
	anim = lerp(currAnim[clipIndex[0]], currAnim[clipIndex[1]], alpha);
	
	world = mul(anim, world);
}

//-----------------------------------------------------------------------------
//CS
//-----------------------------------------------------------------------------
[numthreads(MAX_BONE_COUNT, 1, 1)]
void CS(uint3 id : SV_GroupThreadID)
{
	matrix world = World;
	
	if (BlendingFrames.Mode == 0)
		SetAnimationWorld(world, id);
	else
		SetBlendingWorld(world, id);
	
	world = mul(InputBones[id.x].Transform, world);
    
	OutputBones[id.x].Transform = world;
}

technique11 T0
{
	pass P0
	{
		SetVertexShader(NULL);
		SetPixelShader(NULL);

		SetComputeShader(CompileShader(cs_5_0, CS()));
	}
}