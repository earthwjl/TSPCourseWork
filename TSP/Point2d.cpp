#include "stdafx.h"
#include "Point2d.h"


void CPoint2d::UpdateLastDisplayPoint()
{
	LastDisplayPoint.x = LastDisplayPoint.x  + offsetX;
	LastDisplayPoint.y = LastDisplayPoint.y  + offsetY;
}

void CPoint2d::Zoom(bool up,CPoint pt)
{
	if (up)
	{
		LastDisplayPoint.x *= 1.1;
		LastDisplayPoint.y *= 1.1;
	}
	else
	{
		LastDisplayPoint.x *= 0.9;
		LastDisplayPoint.y *= 0.9;
	}

	DisplayPoint.x = static_cast<int>(LastDisplayPoint.x);
	DisplayPoint.y = static_cast<int>(LastDisplayPoint.y);

}

void CPoint2d::Offset(int dx, int dy)
{
	offsetX = dx;
	offsetY = dy;
	DisplayPoint.x = static_cast<int>(LastDisplayPoint.x) + dx;
	DisplayPoint.y = static_cast<int>(LastDisplayPoint.y) + dy;
}

int CPoint2d::offsetX = 0;
int CPoint2d::offsetY = 0;

double Distance(CPoint2d & p1, CPoint2d & p2)
{
	double xx = (p1.x - p2.x) * (p1.x - p2.x);
	double yy = (p1.y - p2.y)*(p1.y - p2.y);
	return sqrt(xx + yy);
}
