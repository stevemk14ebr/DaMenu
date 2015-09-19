#pragma once

struct Color
{
	float R;
	float G;
	float B;
	float A;
	Color(float R_, float G_, float B_, float A_ = 1.0f)
	{
		R = R_;
		G = G_;
		B = B_;
		A = A_;
	}
	Color()
	{
		R = 0.0f;
		G = 0.0f;
		B = 0.0f;
		A = 0.0f;
	}
};