#include "Framework.h"
#include "SkeletalMeshClip.h"

SkeletalMeshKeyframe* SkeletalMeshClip::Keyframe(wstring boneName)
{
	if (keyframeMap.count(boneName) < 1)
		return nullptr;

	return keyframeMap[boneName];
}
