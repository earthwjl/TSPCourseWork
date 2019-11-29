#pragma once
#ifndef __CGraph__
#define __CGraph__
#include <afx.h>
#include <memory>
#include <vector>
#include "Point2d.h"

class CGraph :public CObject
{
public:
	CGraph() :p_Data(new std::vector<std::vector<double>>()) {}
	CGraph(std::vector<CPoint2d>& pts);
	CGraph(const CGraph& g) { p_Data = g.p_Data; }
	CGraph& operator = (const CGraph&);
	bool IsVisited(size_t i) { if (p_Data && i < p_Data->size() && (*p_Data)[i][i] == 0)return false; else return true; }//用对角线元素表示该点是否访问过
	void SetVisited(size_t i) { if (p_Data&&i < p_Data->size())(*p_Data)[i][i] = 1; }
	void SetUnvisited(size_t i) { if (p_Data&&i < p_Data->size())(*p_Data)[i][i] = 0; }
	void Reset();
	double GetElement(size_t p1, size_t p2) { if (p1 < size() && p2 < size() && p1 != p2)return (*p_Data)[p1][p2]; else return INFINITE; }
	size_t size() { if (p_Data == NULL)return 0; else return p_Data->size(); }
private:
	std::shared_ptr<std::vector<std::vector<double>>>p_Data;
};

#endif