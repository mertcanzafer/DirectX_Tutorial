#pragma once
#include "Drawable.h"

class Box : public Drawable
{
public:
	typedef typename graphics::Graphics Gfx;

	Box(Gfx& gfx,
		std::mt19937& rng,
		std::uniform_real_distribution<float>& adist,
		std::uniform_real_distribution<float>& ddist,
		std::uniform_real_distribution<float>& odist,
		std::uniform_real_distribution<float>& rdist);

	DirectX::XMMATRIX GetTransformXM() const noexcept override;
	void Update(float dt) override;

private:
	// Positional
	float r;
	float roll =		0.0f;
	float pitch =	    0.0f;
	float yaw =			0.0f;
	float theta;
	float phi;
	float chi;
	// Speed (delta/s)
	float droll;
	float dpitch;
	float dyaw;
	float dtheta;
	float dphi;
	float dchi;
};