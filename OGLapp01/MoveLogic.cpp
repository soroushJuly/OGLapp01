#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>

class Movement
{
public:
	void move(bool& direction, float& triOffset)
	{
		if (direction)
		{
			triOffset += increment;
		}
		else
		{
			triOffset -= increment;
		}

		if (abs(triOffset) >= maxOffset)
		{
			direction = !direction;
		}
	}
private:
	float maxOffset = 0.6f;
	float increment = 0.004f;
};