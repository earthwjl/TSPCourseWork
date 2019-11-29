
// TSPView.cpp : CTSPView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "TSP.h"
#endif

#include "TSPDoc.h"
#include "TSPView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTSPView

IMPLEMENT_DYNCREATE(CTSPView, CView)

BEGIN_MESSAGE_MAP(CTSPView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_MOUSEWHEEL()

ON_COMMAND(ID_ALG_Greedy, &CTSPView::OnAlgGreedy)

ON_WM_MOUSEMOVE()
ON_COMMAND(ID_2OPT, &CTSPView::On2opt)
ON_WM_LBUTTONDOWN()
ON_COMMAND(ID_BUTTON_CLEAR, &CTSPView::OnButtonClear)
ON_WM_MBUTTONDOWN()
ON_WM_MBUTTONUP()
ON_COMMAND(ID_GreedyTwoSide, &CTSPView::OnGreedytwoside)
ON_COMMAND(ID_TANXIN, &CTSPView::OnTanxin)
//ON_WM_TIMER()
END_MESSAGE_MAP()

// CTSPView 构造/析构

CTSPView::CTSPView()
	: firstRButtonDown(false)
	, firstRButtonPoint(0)
{
	// TODO: 在此处添加构造代码
}

CTSPView::~CTSPView()
{
}

BOOL CTSPView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CTSPView 绘制

void CTSPView::OnDraw(CDC* pDC)
{
	CTSPDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	//绘制点集
	if (pDoc->m_pNodes->size())
	{
		std::vector<CPoint2d>& curVec = *(pDoc->m_pNodes);
		CBrush brush(RGB(255, 0, 0));
		pDC->SelectObject(brush);
		
		int radius = 5;
		CPoint pt1 = curVec[0].DisplayPoint;
		pDC->Ellipse(pt1.x - radius, pt1.y - radius, pt1.x + radius, pt1.y + radius);
		for (size_t i = 1; i < curVec.size(); ++i)
		{
			CPoint pt2 = curVec[i].DisplayPoint;
			pDC->Ellipse(pt2.x - radius, pt2.y - radius, pt2.x + radius, pt2.y + radius);
			pt1 = pt2;	
		}
	}
	//连接点集
	if (pDoc->m_pResult->size())
	{
		std::shared_ptr<std::vector<size_t>>pRes = pDoc->m_pResult;
		CPoint first = (*pDoc->m_pNodes)[(*pRes)[0]].DisplayPoint;
		CPoint allF = first;
		int i = 1;
		for (; i < pRes->size(); ++i)
		{
			pDC->MoveTo(first);
			CPoint second = (*pDoc->m_pNodes)[(*pRes)[i]].DisplayPoint;
			pDC->LineTo(second);
			first = second;
		}
		ReleaseDC(pDC);
	}
	
}


// CTSPView 打印

BOOL CTSPView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CTSPView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CTSPView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CTSPView 诊断

#ifdef _DEBUG
void CTSPView::AssertValid() const
{
	CView::AssertValid();
}

void CTSPView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTSPDoc* CTSPView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTSPDoc)));
	return (CTSPDoc*)m_pDocument;
}
#endif //_DEBUG


// CTSPView 消息处理程序


BOOL CTSPView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CTSPDoc* pDoc = GetDocument();
	if (pDoc->m_pNodes->empty())
		return CView::OnMouseWheel(nFlags, zDelta, pt);
	std::shared_ptr<std::vector<CPoint2d>>src = pDoc->m_pNodes;
	if (zDelta > 0)
	{
		for (size_t i = 0; i < src->size(); ++i)
		{
			(*src)[i].Zoom(true,pt);
		}
	}
	else
	{
		for (size_t i = 0; i < src->size(); ++i)
		{
			(*src)[i].Zoom(false,pt);
		}
	}
	Invalidate();

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void CTSPView::TextToStatusBar(LPCTSTR Str,int index)
{
	CMainFrame* mainFrm = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	mainFrm->m_wndStatusBar.SetPaneText(index, Str);
}


void CTSPView::OnAlgGreedy()
{
	// TODO: 在此添加命令处理程序代码
	CTSPDoc* pDoc = GetDocument();
	DWORD st = GetTickCount();
	CString cal("计算中...");
	TextToStatusBar(cal, 0);
	pDoc->OnAlgGreedy();
	DWORD end = GetTickCount();
	Invalidate();
	CString str;
	str.Format(_T("贪心算法 耗时 %d ms 距离为%.1lf"), end - st, pDoc->GetPathLen(pDoc->m_pResult));
	TextToStatusBar(str, 0);

}


void CTSPView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (firstRButtonDown)
	{
		CTSPDoc* pDoc = GetDocument();
		HCURSOR hc = LoadCursor(NULL, IDC_SIZEALL);
		::SetCursor(hc);
		std::shared_ptr<std::vector<CPoint2d>>src = pDoc->m_pNodes;
		for (size_t i = 0; i < src->size(); ++i)
		{
			(*src)[i].Offset(point.x - firstRButtonPoint.x, point.y - firstRButtonPoint.y);
		}
		Invalidate();
	}
	CView::OnMouseMove(nFlags, point);
}


void CTSPView::On2opt()
{
	// TODO: 在此添加命令处理程序代码
	CTSPDoc* pDoc = GetDocument();
	DWORD st = GetTickCount();
	CString cal("计算中...");
	TextToStatusBar(cal, 0);
	pDoc->On2opt();
	DWORD end = GetTickCount();
	Invalidate();
	CString str;
	str.Format(_T("2opt 耗时 %d ms 距离为%.1lf"), end - st,pDoc->GetPathLen(pDoc->m_pResult));
	TextToStatusBar(str, 0);
}


void CTSPView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CTSPDoc* pDoc = GetDocument();
	if (pDoc->FileOpened == false)
	{
		CPoint2d pt(0,point.x, point.y);

		pDoc->m_pNodes->push_back(pt);
		pDoc->graph = CGraph(*(pDoc->m_pNodes));
		Invalidate();
	}
	CView::OnLButtonDown(nFlags, point);
}


void CTSPView::OnButtonClear()
{
	// TODO: 在此添加命令处理程序代码
	CTSPDoc* pDoc = GetDocument();
	pDoc->m_pNodes->clear();
	pDoc->m_pResult->clear();
	Invalidate();
}


void CTSPView::OnMButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	HCURSOR hc = LoadCursor(NULL, IDC_SIZEALL);
	::SetCursor(hc);
	firstRButtonDown = true;
	firstRButtonPoint = point;
	CView::OnMButtonDown(nFlags, point);
}


void CTSPView::OnMButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	firstRButtonDown = false;
	HCURSOR hc = LoadCursor(NULL, IDC_ARROW);
	::SetCursor(hc);
	CTSPDoc* pDoc = GetDocument();
	std::shared_ptr<std::vector<CPoint2d>>src = pDoc->m_pNodes;

	for (size_t i = 0; i < src->size(); ++i)
	{
		(*src)[i].UpdateLastDisplayPoint();
	}
	CView::OnMButtonUp(nFlags, point);
}



void CTSPView::OnGreedytwoside()
{
	// TODO: 在此添加命令处理程序代码
	CTSPDoc* pDoc = GetDocument();
	DWORD st = GetTickCount();
	CString cal("计算中...");
	TextToStatusBar(cal, 0);
	pDoc->onAlgTwoGreedy();
	DWORD end = GetTickCount();
	Invalidate();
	CString str;
	str.Format(_T("中间贪心法 耗时 %d ms 距离为%.1lf"), end - st, pDoc->GetPathLen(pDoc->m_pResult));
	TextToStatusBar(str, 0);
}


void CTSPView::OnTanxin()
{
	// TODO: 在此添加命令处理程序代码
	CTSPDoc* pDoc = GetDocument();
	DWORD st = GetTickCount();
	CString cal("计算中...");
	TextToStatusBar(cal, 0);
	
	pDoc->tanxin2();
	
	DWORD end = GetTickCount();
	Invalidate();
	CString str;
	str.Format(_T("双向贪心法 耗时 %d ms 距离为%.1lf"), end - st, pDoc->GetPathLen(pDoc->m_pResult));
	TextToStatusBar(str, 0);
}

