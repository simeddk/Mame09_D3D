#include "Framework.h"
#include "SkeletalMeshClip.h"

SkeletalMeshKeyframe* SkeletalMeshClip::Keyframe(wstring boneName)
{
	if (keyframeMap.count(boneName) < 1) //버그 원인! boneName이 아니라 clipName으로 검색하고 앉아 있었군
		return nullptr;

	return keyframeMap[boneName];
}
