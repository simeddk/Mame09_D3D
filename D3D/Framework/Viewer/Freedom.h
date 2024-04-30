#pragma once

class Freedom : public Camera
{
public:
	Freedom() = default;
	~Freedom() = default;

	void Update() override;

	void Speed(float moveSpeed, float rotationSpeed = 5.f);

private:
	float moveSpeed = 20.f;
	float rotationSpeed = 5.f;
};