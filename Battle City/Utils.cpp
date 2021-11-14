#include "Windows.h"
#include "Utils.h"


bool isKeyDown(int virtualKeyCode)
{
	short keyState = GetAsyncKeyState(virtualKeyCode);
	return ((keyState & 0x8000) > 0);
}

int getRandomInt(int min, int max)
{
	return min + rand() % (max - min + 1);
}

float getRandomFloat(float min, float max)
{
	return (min + ((float)max - min) * (rand() % 101 / 100.0f));
}