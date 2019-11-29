
// TSPDoc.cpp : CTSPDoc ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "TSP.h"
#endif

#include "TSPDoc.h"
#include <propkey.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include <deque>
#include <ctime>
#include <memory>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CTSPDoc

IMPLEMENT_DYNCREATE(CTSPDoc, CDocument)

BEGIN_MESSAGE_MAP(CTSPDoc, CDocument)
ON_COMMAND(ID_2OPT, &CTSPDoc::On2opt)
//ON_COMMAND(ID_MONITUIHUO, &CTSPDoc::OnMonituihuo)
//ON_COMMAND(ID_GreedyTwoSide, &CTSPDoc::OnGreedytwoside)
END_MESSAGE_MAP()


// CTSPDoc ����/����

CTSPDoc::CTSPDoc():m_pNodes(new std::vector<CPoint2d>()),m_pResult(new std::vector<size_t>())
{
	// TODO: �ڴ����һ���Թ������
	FileOpened = false;
	
}

CTSPDoc::~CTSPDoc()
{
}

BOOL CTSPDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}

// CTSPDoc ���л�

void CTSPDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
		if (m_pResult == NULL || m_pResult->empty())
			return;
		for (size_t i = 0; i < m_pResult->size() - 1; ++i)
		{
			size_t id = (*m_pResult)[i];
			CString line;
			line.Format(_T("%d %.1lf %.1lf\n"), id+1, (*m_pNodes)[id].x, (*m_pNodes)[id].y);
			ar.WriteString(line);
		}
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
		CFile* file = ar.GetFile();//��ȡ��ǰ�ļ�
		char* buffer = new char[file->GetLength()];//������ʱ����
		file->Read(buffer, file->GetLength());
		std::stringstream ss(buffer);
		delete[] buffer;

		std::string line;
		bool validData = false;//�ж������Ƿ���Ч
		while (std::getline(ss, line))//��ȡһ������
		{
			std::stringstream localSS(line);
			std::string first;
			localSS >> first;//�õ���һ���ַ���
			
			if (!isdigit(first[0]))//��һ���ַ�������ĸ��ͷ
			{
				if (first == "NODE_COORD_SECTION")
				{
					validData = true;//������Ч
					m_pNodes->clear();//�����һ���ļ����������
					m_pResult->clear();//���֮ǰ��·�����
					FileOpened = true;
				}
				if (first == "EOF")
					break;//�����ļ�β������
				continue;
			}
			else if (validData)//��ʼ��ȡ����
			{
				size_t id = stoi(first);
				double xx, yy;//�õ�x�����y����
				localSS >> xx >> yy;
				m_pNodes->push_back(CPoint2d(id,xx, yy));
			}
			else
			{
				MessageBox(*AfxGetMainWnd(), _T("��֧�ֵ����ݸ�ʽ"), _T("����"), WM_CLOSE);
				ExitProcess(1);//������֧�ֵ��ļ���ʽֱ�ӽ�������
				return;
			}
		}
		if (!m_pNodes->empty())
		{
			graph = CGraph(*m_pNodes);
		}
	}
}

#ifdef SHARED_HANDLERS

// ����ͼ��֧��
void CTSPDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// �޸Ĵ˴����Ի����ĵ�����
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// ������������֧��
void CTSPDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ���ĵ����������������ݡ�
	// ���ݲ���Ӧ�ɡ�;���ָ�

	// ����:     strSearchContent = _T("point;rectangle;circle;ole object;")��
	SetSearchContent(strSearchContent);
}

void CTSPDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CTSPDoc ���

#ifdef _DEBUG
void CTSPDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTSPDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CTSPDoc ����


void CTSPDoc::OnAlgGreedy()
{
	// TODO: �ڴ���������������
	if (m_pNodes->empty())
		return;
	m_pResult->clear();
	graph.Reset();
	size_t i = 0;
	size_t j = 0;
	while (graph.IsVisited(i) == false)
	{
		graph.SetVisited(i);
		size_t minV = 0;

		double minDist = INT_MAX;
		m_pResult->push_back(i);
		for (; j < graph.size(); ++j)
		{
			if (graph.IsVisited(j) == false && graph.GetElement(i, j) < minDist)
			{
				minDist = graph.GetElement(i, j);
				minV = j;
			}
		}
		i = minV;
		j = 0;
	}
	m_pResult->push_back(0);
}


void CTSPDoc::On2opt()
{
	//���֮ǰ���
	if (m_pNodes->empty())
		return;
	m_pResult->clear();
	graph.Reset();
	//̰���㷨������ʼ��
	size_t startP = rand() % (m_pNodes->size());
	size_t i = startP;
	size_t j = 0;
	while (graph.IsVisited(i) == false)
	{
		graph.SetVisited(i);
		size_t minV = 0;

		double minDist = INT_MAX;
		m_pResult->push_back(i);
		for (; j < graph.size(); ++j)
		{
			if (graph.IsVisited(j) == false && graph.GetElement(i, j) < minDist)
			{
				minDist = graph.GetElement(i, j);
				minV = j;
			}
		}

		i = minV;
		j = 0;
	}
	m_pResult->push_back(startP);
	//���õ�����������
	srand(GetTickCount());
	int iterationTimes = 100000;
	const int num = m_pResult->size();

	while (iterationTimes > 0)
	{
		//���ѡȡ������
		int e1 = rand() % num;
		int e2 = rand() % num;
		if (e1 + 1 == num || e2 + 1 == num || e1 + 1 == e2 || e2 + 1 == e1)
			continue;
		//�ߵ����˵�
		size_t pt1 = (*m_pResult)[e1];
		size_t pt2 = (*m_pResult)[e1+1];

		size_t pt3 = (*m_pResult)[e2];
		size_t pt4 = (*m_pResult)[e2+1];
		//����Ƿ񽻲�
		if (graph.GetElement(pt1, pt2) + graph.GetElement(pt3, pt4) > graph.GetElement(pt1, pt3) + graph.GetElement(pt2, pt4))
		{
			//��ת�м䲿��
			int beg = e1 + 1;
			int end = e2;

			while (beg < end)
			{
				int tmp = (*m_pResult)[beg];
				(*m_pResult)[beg] = (*m_pResult)[end];
				(*m_pResult)[end] = tmp;
				beg++; end--;
			}
			//���õ�������
			iterationTimes = 100000;
		}
		else
			iterationTimes--;
	}
}

double CTSPDoc::GetPathLen(std::shared_ptr<std::vector<size_t>> ptr)
{
	// TODO: �ڴ˴����ʵ�ִ���.
	double dist = 0.0;
	if (ptr&&ptr->size())
	{
		for (size_t i = 1; i < ptr->size(); ++i)
		{
			dist += graph.GetElement((*ptr)[i], (*ptr)[i - 1]);
		}
	}
	return dist;
}


void CTSPDoc::onAlgTwoGreedy()
{
	// TODO: �ڴ˴����ʵ�ִ���.
	m_pResult->clear();//���֮ǰ�����ݣ�����ͼ
	graph.Reset();

	size_t startID = 0;
	size_t left = startID, right = startID;//��ʼ��ID
	std::vector<size_t>leftPart, rightPart;//�����Ұ��
	graph.SetVisited(left);

	while (1)
	{
		//����Ѱ�������
		double minDist = DBL_MAX;
		double minID = left;
		for (size_t i = 0; i < m_pNodes->size(); ++i)
		{
			if (i != left && graph.IsVisited(i) == false && graph.GetElement(left, i) < minDist)
			{
				minDist = graph.GetElement(left, i);
				minID = i;
			}
		}
		if (left == minID)
			break;
		leftPart.push_back(minID);
		left = minID;
		graph.SetVisited(left);
		//�Ҳ�Ѱ�������
		minDist = DBL_MAX;
		minID = right;
		for (size_t i = 0; i < m_pNodes->size(); ++i)
		{
			if (i != right && graph.IsVisited(i) == false && graph.GetElement(i, right) < minDist)
			{
				minDist = graph.GetElement(right, i);
				minID = i;
			}
		}
		if (right == minID)
			break;
		rightPart.push_back(minID);
		right = minID;
		graph.SetVisited(right);
	}
	m_pResult->insert(m_pResult->begin(), leftPart.rbegin(), leftPart.rend());
	m_pResult->push_back(startID);
	m_pResult->insert(m_pResult->end(), rightPart.begin(), rightPart.end());
	m_pResult->push_back(m_pResult->front());
}



void CTSPDoc::tanxin2()
{

	// TODO: �ڴ˴����ʵ�ִ���.
	m_pResult->clear();
	graph.Reset();

	size_t startP = 0;
	size_t left = startP, right = startP;
	std::deque<size_t>ans;
	ans.push_back(startP);
	graph.SetVisited(startP);
	while (ans.size() < m_pNodes->size())
	{
		size_t localLeft = left, localRight = right;
		double minDist = DBL_MAX;
		for (size_t i = 0; i < m_pNodes->size(); ++i)
		{
			if (i != localLeft && graph.IsVisited(i) == false && graph.GetElement(i, left) < minDist)
			{
				minDist = graph.GetElement(i, left);
				localLeft = i;
			}
		}
		graph.SetVisited(localLeft);

		minDist = DBL_MAX;

		for (size_t i = 0; i < m_pNodes->size(); ++i)
		{
			if (i != localRight && graph.IsVisited(i) == false && graph.GetElement(i, right) < minDist)
			{
				minDist = graph.GetElement(i, right);
				localRight = i;
			}
		}
		graph.SetVisited(localRight);

		if (localLeft == left)
		{
			if (localRight == right)
				break;
			else
			{
				ans.push_back(localRight);
				right = localRight;
			}
		}
		else if (localRight == right)
		{
			ans.push_front(localLeft);
			left = localLeft;
		}
		else
		{
			if (graph.GetElement(localLeft, ans.front()) < graph.GetElement(localRight, ans.back()))
			{
				left = localLeft;
				ans.push_front(localLeft);
				graph.SetUnvisited(localRight);
			}
			else
			{
				right = localRight;
				ans.push_back(localRight);
				graph.SetUnvisited(localLeft);
			}
		}
	}
	m_pResult->insert(m_pResult->begin(), ans.begin(), ans.end());
	m_pResult->push_back(m_pResult->front());
}
