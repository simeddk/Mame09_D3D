#pragma once

#include "Systems/IExecute.h"

class GetHeightDemo : public IExecute
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
	Shader* shader;
	Terrain* terrain;
	
	Shader* triShader;
	//역삼각형 그리기 ^^

};