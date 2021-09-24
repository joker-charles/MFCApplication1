
// MFCApplication1View.cpp: CMFCApplication1View 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "MFCApplication1.h"
#endif

#include "MFCApplication1Doc.h"
#include "MFCApplication1View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCApplication1View

IMPLEMENT_DYNCREATE(CMFCApplication1View, CView)

BEGIN_MESSAGE_MAP(CMFCApplication1View, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMFCApplication1View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CMFCApplication1View 构造/析构

CMFCApplication1View::CMFCApplication1View() noexcept
{
	// TODO: 在此处添加构造代码

}

CMFCApplication1View::~CMFCApplication1View()
{
}

BOOL CMFCApplication1View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CMFCApplication1View 绘图
inline void ShowChar1(CDC* pDC, int x0, int y0, BYTE mark[], int n, COLORREF color)
{
	long t[32];
	t[n - 1] = 1;
	for (int i = n - 2; i >= 0; i--)
	{
		t[i] = t[i + 1] * 2;
	}
	for (int y = 0; y < n; y++) {
		for (int x = 0; x < n; x++) {
			if (mark[y] & t[x]) {
				pDC->SetPixel(x + x0, y + y0, color);
			}
		}
	}
}
inline void ShowChar2(CDC* pDC, int x0, int y0, int bh[], COLORREF color) {
	int i = 0, xs = bh[i + 1], ys = bh[i + 2], x, y;
	CPen pen(PS_SOLID, 1, color);
	auto oldPen = pDC->SelectObject(&pen);
	/**
	* 状态机0移动
	* 1画线
	* 2画曲线
	*/
	while (bh[i] != -1)
	{
		if (bh[i] == 0) {
			xs = bh[i + 1];
			ys = bh[i + 2];
			i += 3;
		}
		else if (bh[i] == 1) {
			pDC->MoveTo(xs + x0, ys + y0);
			pDC->LineTo(bh[i + 1] + x0, bh[i + 2] + y0);
			xs = bh[i + 1];
			ys = bh[i + 2];
			i += 3;
		}
		else if (bh[i] == 2) {
			int _xm = bh[i + 1];
			int _ym = bh[i + 2];
			int _xe = bh[i + 3];
			int _ye = bh[i + 4];
			float ax = 2.0f * (_xe - 2 * _xm + xs);
			float ay = 2.0f * (_ye - 2 * _ym + ys);
			float bx = 4.0f * _xm - _xe - 3 * xs;
			float by = 4.0f * _ym - _ye - 3 * ys;
			int cx = xs;
			int cy = ys;
			for (float t = 0.05f; t <= 1.0001f; t += 0.05f) {
				x = ax * t * t + bx * t + cx;
				y = ay * t * t + by * t + cy;
				pDC->MoveTo(xs + x0, ys + y0);
				pDC->LineTo(x + x0, y + y0);
				xs = x;
				ys = y;
			}
			xs = _xe;
			ys = _ye;
			i += 5;
		}
	}
	pDC->SelectObject(oldPen);
}
inline void ShowChar3(CDC* pDC, int x0, int y0, float xb, float yb, int bh[], COLORREF color) {
	int i = 0,
		x1 = 0,
		y1 = 0,
		x2, y2, bz{}, lens, d;
	CPen pen(PS_SOLID, 1, color);
	auto oldPen = pDC->SelectObject(&pen);
	while (bh[i] != 0) {
		if (bh[i] == 1 || bh[i] == 2) {
			bz = bh[i];
		}
		else {
			d = bh[i] & 15;
			lens = bh[i] >> 4;
			/*char buf[124]{0};
			_itoa_s(d,buf,10);
			CString mes{buf};
			AfxMessageBox(mes);*/
		
			switch (d) {
			case 0:
				x2 = x1 + xb * lens;
				y2 = y1;
				break;
			case 1:
				x2 = x1 + xb * lens;
				y2 = y1 + yb * lens / 2;
				break;
			case 2:
				x2 = x1 + xb * lens;
				y2 = y1 + yb * lens;
				break;
			case 3:
				x2 = x1 + xb * lens / 2;
				y2 = y1 + yb * lens;
				break;
			case 4:
				x2 = x1;
				y2 = y1 + yb * lens;
				break;
			case 5:
				x2 = x1 - xb * lens/2;
				y2 = y1 + yb * lens;
				break;
			case 6:
				x2 = x1 - xb * lens;
				y2 = y1 + yb * lens;
				break;
			case 7:
				x2 = x1 - xb * lens;
				y2 = y1 + yb * lens / 2;
				break;
			case 8:
				x2 = x1 - xb * lens;
				y2 = y1;
				break;
			case 9:
				x2 = x1 - xb * lens;
				y2 = y1 - yb * lens / 2;
				break;
			case 10:
				x2 = x1 - xb * lens;
				y2 = y1 - yb * lens; 
				break;
			case 11:
				x2 = x1 - xb * lens / 2;
				y2 = y1 - yb * lens;
				break;
			case 12:
				x2 = x1;
				y2 = y1 - yb * lens; 
				break;
			case 13:
				x2 = x1 + xb * lens / 2;
				y2 = y1 - yb * lens; 
				break;
			case 14:
				x2 = x1 + xb * lens;
				y2 = y1 - yb * lens; 
				break;
			case 15:
				x2 = x1 + xb * lens;
				y2 = y1- yb * lens / 2; 
				break;
			}
			if (bz != 2) {
#ifdef translate
				pDC->MoveTo(x1+x0, -y1+y0);
				pDC->LineTo(x2+x0, -y2+y0);
#endif
				pDC->MoveTo(x1+x0, y1+y0);
				pDC->LineTo(x2+x0, y2+y0);
			}
			x1 = x2;
			y1 = y2;
		}
		++i;
	}
	pDC->SelectObject(oldPen);
}
void CMFCApplication1View::OnDraw(CDC* pDC)
{
	CMFCApplication1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	BYTE r[8]{ 0xFC,0X66,0X66,0x7c,0x66,0x66,0xfc,0x00};
	ShowChar1(pDC, 50, 50, r, 8, RGB(255, 0, 0));
	int bh[] = {
		0,10,10,
		1,10,110,
		1,60,110,
		2,80,85,60,60,
		2,80,35,60,10,
		1,10,10,
		0,10,60,
		1,60,60,
		-1 };
	ShowChar2(pDC, 100, 100, bh, RGB(0, 0, 0));
#ifdef translate
	//手动映射x,y到左上角为坐标原点
	int bh2[]={
	1,0x24,0x49,0x41,0x44,0x38,0x30,0x44,
	2,0x20,
	1,0x5c,0x31,0x39,0x5c,0x40,0x24,0
	};//未变换
	ShowChar3(pDC, 100, 100, 5, 5, bh2, RGB(0, 0, 0));
#endif
	int bh1[]={
	1,0x2c,0x47,0x4f,0x4c,0x38,0x30,0x4c,
	2,0x20,
	1,0x54,0x3f,0x37,0x54,0x40,0x2c,0
	};
	
	ShowChar3(pDC, 100, 100, 5, 5, bh1, RGB(0, 0, 0));
}


// CMFCApplication1View 打印


void CMFCApplication1View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMFCApplication1View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CMFCApplication1View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CMFCApplication1View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CMFCApplication1View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMFCApplication1View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMFCApplication1View 诊断

#ifdef _DEBUG
void CMFCApplication1View::AssertValid() const
{
	CView::AssertValid();
}

void CMFCApplication1View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCApplication1Doc* CMFCApplication1View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCApplication1Doc)));
	return (CMFCApplication1Doc*)m_pDocument;
}
#endif //_DEBUG


// CMFCApplication1View 消息处理程序
