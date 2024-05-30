#include "stdafx.h"
#include "TextureBufferDemo.h"

void TextureBufferDemo::Initialize()
{
	shader = new Shader(L"19_TextureBuffer.fxo");
	texture = new Texture(L"Environment/Iconofsin_large.png");

	textureBuffer = new TextureBuffer(texture->GetTexture());
	shader->AsSRV("Input")->SetResource(textureBuffer->SRV());
	shader->AsUAV("Output")->SetUnorderedAccessView(textureBuffer->UAV());

	canvas = new Canvas();
	canvas->GetTransform()->Scale(D3D::Width(), D3D::Height(), 1);
	canvas->GetTransform()->Position(D3D::Width() * 0.5f, D3D::Height() * 0.5f, 0);
	canvas->SRV(textureBuffer->OutputSRV());
}

void TextureBufferDemo::Update()
{
	UINT width = textureBuffer->Width();
	UINT height = textureBuffer->Height();

	float x = ((float)width / 32.0f) < 1.0f ? 1.0f : ((float)width / 32.0f);
	float y = ((float)height / 32.0f) < 1.0f ? 1.0f : ((float)height / 32.0f);

	UINT groupX = (UINT)ceilf(x);
	UINT groupY = (UINT)ceilf(y);

	if (ImGui::Button("Pass0"))
		shader->Dispatch(0, 0, groupX, groupY, 1);

	if (ImGui::Button("Pass1"))
		shader->Dispatch(0, 1, groupX, groupY, 1);

	canvas->Update();
}

void TextureBufferDemo::Render()
{
	canvas->Render();
}
