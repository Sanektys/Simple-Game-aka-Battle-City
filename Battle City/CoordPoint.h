#pragma once


struct CoordPoint
{
	void setCoordPoint(class GameObject* modifyObject,
		               enum class typeCoordPoint typePoint);

	float coordY = 0.0f;
	float coordX = 0.0f;
	float width  = 0.1f;
	float height = 0.1f;
};


enum class typeCoordPoint
{
	TOP_LEFT,
	TOP_RIGHT,
	BOTTOM_RIGHT,
	BOTTOM_LEFT
};