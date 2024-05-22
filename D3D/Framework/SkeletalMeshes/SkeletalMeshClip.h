#pragma once

//1 Bone, 1 Frame
struct SkeletalMeshKeyframeData
{
	float Frame;

	Vector3 Scale;
	Quaternion Rotation;
	Vector3 Translation;
};

//1 Bone , All Frames
struct SkeletalMeshKeyframe
{
	wstring BoneName;
	vector<SkeletalMeshKeyframeData> Transforms;
};

//All bones, All Frames(Final Data)
class SkeletalMeshClip
{
public:
	friend class SkeletalMesh;

private:
	SkeletalMeshClip() = default;
	~SkeletalMeshClip() = default;

public:
	UINT FrameCount() { return frameCount; }
	float FrameRate() { return frameRate; }

	SkeletalMeshKeyframe* Keyframe(wstring boneName);

private:
	wstring name;

	UINT frameCount;
	float frameRate;

	unordered_map<wstring, SkeletalMeshKeyframe*> keyframeMap;
};