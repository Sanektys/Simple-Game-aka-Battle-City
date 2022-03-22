#include <Windows.h>
#include <random>

#include "Utils.h"


extern std::mt19937 random;

bool isKeyDown(int virtualKeyCode)
{
	short keyState = GetAsyncKeyState(virtualKeyCode);
	return ((keyState & 0x8000) > 0);
}

int getRandomInt(int min, int max)
{
	return min + random() % (max - min + 1);
}

float getRandomFloat(float min, float max)
{
	return (min + ((float)max - min) * (random() % 101 / 100.0f));
}