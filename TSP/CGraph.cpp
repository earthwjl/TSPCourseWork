#include "stdafx.h"
#include "CGraph.h"

CGraph::CGraph(std::vector<CPoint2d>& pts)
{
	p_Data.reset(new std::vector<std::vector<double>>(pts.size(), std::vector<double>(pts.size(), 0)));
	for (size_t i = 0; i < pts.size(); ++i)
	{
		for (size_t j = 0; j < pts.size(); ++j)
		{
			if (i != j)
			{
				(*p_Data)[i][j] = Distance(pts[i], pts[j]);
			}
		}
	}
}

CGraph & CGraph::operator=(const CGraph & g)
{
	// TODO: 在此处插入 return 语句
	*p_Data = *(g.p_Data);
	g.p_Data->clear();
	return *this;
}

void CGraph::Reset()
{
	for (size_t i = 0; i < p_Data->size(); ++i)
		(*p_Data)[i][i] = 0;
}
