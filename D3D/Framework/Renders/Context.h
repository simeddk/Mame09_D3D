#pragma once

class Context
{
public:
	static Context* Get();
	static void Create();
	static void Delete();

private:
	Context();
	~Context();

public:
	void ResizeScreen();

	void Update();
	void Render();

	D3DXMATRIX View();
	D3DXMATRIX Projection();
	Camera* GetCamera() { return camera; }

	class Perspective* GetPerspective() { return perspective; }
	class Viewport* GetViewport() { return viewport; }

private:
	static Context* instance;

private:
	class Perspective* perspective;
	class Viewport* viewport;

	Camera* camera;
};