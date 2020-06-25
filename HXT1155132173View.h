
// HXT1155132173View.h: CHXT1155132173View 类的接口
//

#pragma once


class CHXT1155132173View : public CView
{
protected: // 仅从序列化创建
	CHXT1155132173View() noexcept;
	DECLARE_DYNCREATE(CHXT1155132173View)

// 特性
public:
	CHXT1155132173Doc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CHXT1155132173View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	int Cx;
	HGLRC m_hRC;
	int Cy;
	bool SetupPixelFormat();
	void SetupViewingFrustum(double aspect);
	void SetupViewingTransform();
	void RenderScene();
	void Lighting();
	void Material();
	void Line();
	

	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	float xtrans;
	float ytrans;
	float ztrans;
	CPoint current_point; 
	CPoint last_point;  
	float xrot; 
	float yrot;
	vector <float> vnorms;
	vector <float> verts;


	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	float m_fScale;
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnFileOpen();
	afx_msg void OnLineTransline();
	afx_msg void OnTestFill();
};

#ifndef _DEBUG  // HXT1155132173View.cpp 中的调试版本
inline CHXT1155132173Doc* CHXT1155132173View::GetDocument() const
   { return reinterpret_cast<CHXT1155132173Doc*>(m_pDocument); }
#endif

