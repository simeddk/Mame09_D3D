#pragma once

#include "Systems/IExecute.h"

class ThreadDemo : public IExecute
{
public:
	virtual void Initialize() override;
	virtual void Destroy() override {};
	virtual void Update() override;
	virtual void PreRender() override {};
	virtual void Render() override {};
	virtual void PostRender() override {};
	virtual void ResizeScreen() override {};

private:
	void Loop();

	void Loop1();
	void Loop2();
	void MultiThread();

	void SharedResource();

	void RaceCondition(int& count);
	void LoopQuater();

private:
	float progress = 0.f;
};