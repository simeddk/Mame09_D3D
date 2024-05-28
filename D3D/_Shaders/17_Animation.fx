#include "00_Global.fx"
#include "00_Light.fx"

float3 LightDirection;

//Reder
struct VertexInput
{
	float4 Position : Position;
	float2 Uv : Uv;
	float3 Normal : Normal;
	float3 Tangent : Tangent;
	float4 BlendIndices : BlendIndices;
	float4 BlendWeights : BlendWeights;
};

struct VertexOutput
{
	float4 Position : SV_Position;
	float2 Uv : Uv;
	float3 Normal : Normal;
};

#define MAX_BONE_COUNT 250
cbuffer CB_Bones
{
	Matrix BoneTransforms[MAX_BONE_COUNT]; //Component(Bone) Space

	uint BoneIndex;
};

Texture2DArray TransformsMap;

//-----------------------------------------------------------------------------
//SetAnimationWorld (TweenMode)
//-----------------------------------------------------------------------------
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

void SetAnimationWorld(inout matrix world, VertexInput input)
{
	float indices[4] = { input.BlendIndices.x, input.BlendIndices.y, input.BlendIndices.z, input.BlendIndices.w };
	float weights[4] = { input.BlendWeights.x, input.BlendWeights.y, input.BlendWeights.z, input.BlendWeights.w };
	
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
	
	[unroll(4)]
	for (int i = 0; i < 4; i++)
	{
		//-----------------------------------------------------------------
		//Current Clip
		//-----------------------------------------------------------------
		//Current Frame
		c0 = TransformsMap.Load(int4(indices[i] * 4 + 0, currFrame[0], clip[0], 0)); //_11_12_13_14
		c1 = TransformsMap.Load(int4(indices[i] * 4 + 1, currFrame[0], clip[0], 0)); //_21_22_23_24
		c2 = TransformsMap.Load(int4(indices[i] * 4 + 2, currFrame[0], clip[0], 0)); //_31_32_33_34
		c3 = TransformsMap.Load(int4(indices[i] * 4 + 3, currFrame[0], clip[0], 0)); //_41_42_43_44
		curr = matrix(c0, c1, c2, c3);
		
		//Next Frame
		n0 = TransformsMap.Load(int4(indices[i] * 4 + 0, nextFrame[0], clip[0], 0)); //_11_12_13_14
		n1 = TransformsMap.Load(int4(indices[i] * 4 + 1, nextFrame[0], clip[0], 0)); //_21_22_23_24
		n2 = TransformsMap.Load(int4(indices[i] * 4 + 2, nextFrame[0], clip[0], 0)); //_31_32_33_34
		n3 = TransformsMap.Load(int4(indices[i] * 4 + 3, nextFrame[0], clip[0], 0)); //_41_42_43_44
		next = matrix(n0, n1, n2, n3);
		
		//Inter Frame Lerp
		currAnim = lerp(curr, next, time[0]);
		
		//-----------------------------------------------------------------
		//Next Clip
		//-----------------------------------------------------------------
		[flatten]
		if(clip[1] > -1)
		{
			//Current Frame
			c0 = TransformsMap.Load(int4(indices[i] * 4 + 0, currFrame[1], clip[1], 0)); //_11_12_13_14
			c1 = TransformsMap.Load(int4(indices[i] * 4 + 1, currFrame[1], clip[1], 0)); //_21_22_23_24
			c2 = TransformsMap.Load(int4(indices[i] * 4 + 2, currFrame[1], clip[1], 0)); //_31_32_33_34
			c3 = TransformsMap.Load(int4(indices[i] * 4 + 3, currFrame[1], clip[1], 0)); //_41_42_43_44
			curr = matrix(c0, c1, c2, c3);
		
			//Next Frame
			n0 = TransformsMap.Load(int4(indices[i] * 4 + 0, nextFrame[1], clip[1], 0)); //_11_12_13_14
			n1 = TransformsMap.Load(int4(indices[i] * 4 + 1, nextFrame[1], clip[1], 0)); //_21_22_23_24
			n2 = TransformsMap.Load(int4(indices[i] * 4 + 2, nextFrame[1], clip[1], 0)); //_31_32_33_34
			n3 = TransformsMap.Load(int4(indices[i] * 4 + 3, nextFrame[1], clip[1], 0)); //_41_42_43_44
			next = matrix(n0, n1, n2, n3);
		
			//Inter Frame Lerp
			nextAnim = lerp(curr, next, time[1]);
				
			//Inter Clip Lerp
			currAnim = lerp(currAnim, nextAnim, TweenFrames.TweenTime);
		}
		
		
		//Add Skinning Weigths
		transform += mul(weights[i], currAnim);
	};
	
	world = mul(transform, world);
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

void SetBlendingWorld(inout matrix world, VertexInput input)
{
	float indices[4] = { input.BlendIndices.x, input.BlendIndices.y, input.BlendIndices.z, input.BlendIndices.w };
	float weights[4] = { input.BlendWeights.x, input.BlendWeights.y, input.BlendWeights.z, input.BlendWeights.w };
	
	float4 c0, c1, c2, c3;
	float4 n0, n1, n2, n3;
	
	matrix transform = 0;
	matrix curr = 0, next = 0;
	matrix currAnim[3];
	matrix anim;
	
	[unroll(4)]
	for (int i = 0; i < 4; i++)
	{
		[unroll(3)]
		for (int j = 0; j < 3; j++)
		{
			//Current Frame
			c0 = TransformsMap.Load(int4(indices[i] * 4 + 0, BlendingFrames.Clips[j].CurrFrame, BlendingFrames.Clips[j].Clip, 0)); //_11_12_13_14
			c1 = TransformsMap.Load(int4(indices[i] * 4 + 1, BlendingFrames.Clips[j].CurrFrame, BlendingFrames.Clips[j].Clip, 0)); //_21_22_23_24
			c2 = TransformsMap.Load(int4(indices[i] * 4 + 2, BlendingFrames.Clips[j].CurrFrame, BlendingFrames.Clips[j].Clip, 0)); //_31_32_33_34
			c3 = TransformsMap.Load(int4(indices[i] * 4 + 3, BlendingFrames.Clips[j].CurrFrame, BlendingFrames.Clips[j].Clip, 0)); //_41_42_43_44
			curr = matrix(c0, c1, c2, c3);
		
			//Next Frame
			n0 = TransformsMap.Load(int4(indices[i] * 4 + 0, BlendingFrames.Clips[j].NextFrame, BlendingFrames.Clips[j].Clip, 0)); //_11_12_13_14
			n1 = TransformsMap.Load(int4(indices[i] * 4 + 1, BlendingFrames.Clips[j].NextFrame, BlendingFrames.Clips[j].Clip, 0)); //_21_22_23_24
			n2 = TransformsMap.Load(int4(indices[i] * 4 + 2, BlendingFrames.Clips[j].NextFrame, BlendingFrames.Clips[j].Clip, 0)); //_31_32_33_34
			n3 = TransformsMap.Load(int4(indices[i] * 4 + 3, BlendingFrames.Clips[j].NextFrame, BlendingFrames.Clips[j].Clip, 0)); //_41_42_43_44
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
		
		//Add Skinning Weigths
		transform += mul(weights[i], anim);
	};
	
	world = mul(transform, world);
}

//-----------------------------------------------------------------------------
//VS
//-----------------------------------------------------------------------------
VertexOutput VS(VertexInput input)
{
	VertexOutput output;
	
	if (BlendingFrames.Mode == 0)
		SetAnimationWorld(World, input);
	else
		SetBlendingWorld(World, input);
	
	output.Position = WorldPosition(input.Position);
	output.Position = ViewProjection(output.Position);

	output.Normal = WorldNormal(input.Normal);

	output.Uv = input.Uv;

	return output;
}

//-----------------------------------------------------------------------------
//PS
//-----------------------------------------------------------------------------
float4 PS_Diffuse(VertexOutput input) :	SV_Target
{
	float3 normal = normalize(input.Normal);
	float lambert = saturate(dot(normal, -LightDirection));

	float4 diffuseColor = DiffuseMap.Sample(LinearSampler, input.Uv);
	return diffuseColor * lambert;
}

float4 PS_WireFrame(VertexOutput input) : SV_Target
{
	return float4(0.5f, 0.5f, 0.5f, 1);
}

//-----------------------------------------------------------------------------
//Pass
//-----------------------------------------------------------------------------
technique11 T0
{
	P_VP(P0, VS, PS_Diffuse)
	P_RS_VP(P1, FillMode_WireFrame, VS, PS_WireFrame)
}