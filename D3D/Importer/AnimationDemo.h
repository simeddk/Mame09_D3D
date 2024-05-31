#pragma once

#include "Systems/IExecute.h"

class AnimationDemo : public IExecute
{
public:
	virtual void Initialize() override;
	virtual void Destroy() override;
	virtual void Update() override;
	virtual void PreRender() override {};
	virtual void Render() override;
	virtual void PostRender() override {};
	virtual void ResizeScreen() override {};

private:
	void Kachujin();

private:
	Shader* shader;

	SkeletalMeshAnimator* kachujin = nullptr;

	struct ColliderObject* colliderObject;
	Matrix bones[MAX_BONE_COUNT];

	Shader* weaponShader;
	SkeletalMeshRenderer* weapon = nullptr;
	Transform* weaponInitTransform = nullptr;
};