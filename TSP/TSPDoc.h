
// TSPDoc.h : CTSPDoc 类的接口
//
#include <memory>
#include <vector>
#include "Point2d.h"
#include "CGraph.h"

#pragma once


class CTSPDoc : public CDocument
{
protected: // 仅从序列化创建
	CTSPDoc();
	DECLARE_DYNCREATE(CTSPDoc)

// 特性
public:

// 操作
public:

// 重写
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 实现
public:
	virtual ~CTSPDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 用于为搜索处理程序设置搜索内容的 Helper 函数
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	// 存储结点
	std::shared_ptr<std::vector<CPoint2d>> m_pNodes;
	void OnAlgGreedy();
	std::shared_ptr<std::vector<size_t>> m_pResult;
	CGraph graph;
	afx_msg void On2opt();
	// 当前是否存在打开的文件
	bool FileOpened;
	double GetPathLen(std::shared_ptr<std::vector<size_t>> ptr);
	void onAlgTwoGreedy();
	void tanxin2();
};
