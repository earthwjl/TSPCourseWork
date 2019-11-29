#pragma once
#include "afx.h"

class CGraph;

class CPoint2d:
	public CObject
{
public:
	friend class CGraph;
	friend double Distance(CPoint2d& p1, CPoint2d& p2);
public:
	CPoint2d(size_t idd, double x, double y) :id(idd) { this->x = x; this->y = y; DisplayPoint = CPoint((int)x, (int)y); LastDisplayPoint.x = x, LastDisplayPoint.y = y; }
	CPoint2d() :x(0.0), y(0.0), id(0) { DisplayPoint = CPoint(0, 0); LastDisplayPoint.x = x; LastDisplayPoint.y = y; }
	CPoint2d(const CPoint2d& pt) :x(pt.x), y(pt.y), id(pt.id) { DisplayPoint = pt.DisplayPoint; LastDisplayPoint = pt.LastDisplayPoint; }
	CPoint2d& operator=(const CPoint2d& pt)
	{
		if (this == &pt)
			return *this;
		this->x = pt.x;
		this->y = pt.y;
		this->id = pt.id;
		this->DisplayPoint = pt.DisplayPoint;
		this->LastDisplayPoint = pt.LastDisplayPoint;
		return *this;
	}
	double x;
	double y;
	size_t GetID() { return id; }
	void UpdateLastDisplayPoint();

	bool operator == (CPoint2d& p) { return this->id == p.GetID(); }
	static void ResetOffset() { offsetX = 0; offsetY = 0; }
	static void SetOffset(int dx, int dy) { offsetX = dx; offsetY = dy; }
private:
	size_t id;
public:
	struct{
		double x;
		double y;
	}LastDisplayPoint;

	static int offsetX;
	static int offsetY;
	void Zoom(bool up,CPoint center);
	void Offset(int dx, int dy);
	CPoint DisplayPoint;
};
double Distance(CPoint2d& p1, CPoint2d& p2);


