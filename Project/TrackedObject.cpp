#pragma once
#include "TrackedObject.h"

TrackedObject::TrackedObject(Point2f point)
{
	this->point = point;
}

int TrackedObject::getX()
{
	return (int)point.x;
}

int TrackedObject::getY()
{
	return (int)point.y;
}

Point2f TrackedObject::getPoint()
{
	return point;
}