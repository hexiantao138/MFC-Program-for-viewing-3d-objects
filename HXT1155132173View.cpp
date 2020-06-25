
// HXT1155132173View.cpp: CHXT1155132173View 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "HXT1155132173.h"
#endif

#include "HXT1155132173Doc.h"
#include "HXT1155132173View.h"
ifstream in;
ofstream out;



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHXT1155132173View

IMPLEMENT_DYNCREATE(CHXT1155132173View, CView)

BEGIN_MESSAGE_MAP(CHXT1155132173View, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_KEYDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_COMMAND(ID_FILE_OPEN, &CHXT1155132173View::OnFileOpen)
	ON_COMMAND(ID_LINE_TRANSLINE, &CHXT1155132173View::OnLineTransline)
	ON_COMMAND(ID_TEST_FILL, &CHXT1155132173View::OnTestFill)
END_MESSAGE_MAP()

// CHXT1155132173View 构造/析构

CHXT1155132173View::CHXT1155132173View() noexcept
{
	// TODO: 在此处添加构造代码
	m_hRC = 0;
	Cx = 0;
	Cy = 0;
	xtrans = 0.0;
	ytrans = 0.0;
	ztrans = 0.0;
	xrot = 0.0;
	yrot = 0.0;
	m_fScale = 1.0;

}

CHXT1155132173View::~CHXT1155132173View()
{
}

BOOL CHXT1155132173View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CHXT1155132173View 绘图

void CHXT1155132173View::OnDraw(CDC* pDC)
{
	CHXT1155132173Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	wglMakeCurrent(pDC->m_hDC, m_hRC);
	glViewport(0, 0, Cx, Cy); 
	SetupViewingFrustum((double)Cx / (double)Cy);
	SetupViewingTransform();
	Lighting();
	Material();
	RenderScene();
	wglMakeCurrent(pDC->m_hDC, NULL);
}


// CHXT1155132173View 打印

BOOL CHXT1155132173View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CHXT1155132173View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CHXT1155132173View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CHXT1155132173View 诊断

#ifdef _DEBUG
void CHXT1155132173View::AssertValid() const
{
	CView::AssertValid();
}

void CHXT1155132173View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CHXT1155132173Doc* CHXT1155132173View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CHXT1155132173Doc)));
	return (CHXT1155132173Doc*)m_pDocument;
}
#endif //_DEBUG


// CHXT1155132173View 消息处理程序
bool CHXT1155132173View::SetupPixelFormat()
{
	static PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR), // Size of this pfd  
		1, // Version number  
		PFD_DRAW_TO_WINDOW | // Support window  
		PFD_SUPPORT_OPENGL | // Support OpenGL 
		PFD_TYPE_RGBA,
		32, // 32-bit color depth  
		0, 0, 0, 0, 0, 0, // Color bits ignored  
		0, // No alpha buffer 
		0, // Shift bit ignored  
		0, // No accumulation buffer  
		0, 0, 0, 0, // Accum bits ignored 
		16, // 16-bit z-buffer   
		0, // No stencil buffer  
		0, // No auxiliary buffer  
		PFD_MAIN_PLANE, // Main layer 
		0, // Reserved 
		0, 0, 0 // Layer masks ignored 
	};
	int pixelformat;
	CClientDC clientDC(this);
	if ((pixelformat = ::ChoosePixelFormat(clientDC.m_hDC, &pfd)) == 0)
		return false;
	if (!(::SetPixelFormat(clientDC.m_hDC, pixelformat, &pfd)))
		return false;
	m_hRC = wglCreateContext(clientDC.m_hDC);
	return true;
}




void CHXT1155132173View::OnDestroy()
{
	CView::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	::wglDeleteContext(m_hRC);
}


int CHXT1155132173View::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	if (!SetupPixelFormat()) 
		return -1;


	return 0;
}


void CHXT1155132173View::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	Cx = cx; 
	Cy = cy; 
	InvalidateRect(0, FALSE);


}

void CHXT1155132173View::SetupViewingFrustum(double aspect)
{ 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, aspect, 1, 100.0); 
}

void CHXT1155132173View::SetupViewingTransform() 
{ 
	glMatrixMode(GL_MODELVIEW); 
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 10.0, 
		      0.0, 0.0, 0.0, 
		      0.0, 1.0, 0.0);
}

void CHXT1155132173View::RenderScene() 
{ 
	//clear background color of OpenGL scene 
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f ); 
	//clear the screen and the depth buffer 
	glClear( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); 
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);     
	/*OnLineTransline();*/
	glShadeModel(GL_SMOOTH);
	glTranslatef(xtrans, ytrans, ztrans);
	glRotatef(xrot, 1, 0, 0);
	glRotatef(yrot, 0, 1, 0);
	glScalef(m_fScale, m_fScale, m_fScale);
	
	//glNormal3f(0.0, 0.0, 1.0);
 //   glBegin(GL_TRIANGLES); 
	//glColor3f(1.0, 0.0, 0.0); // Red  
	//glVertex3f(0.0f, 2.0f, 0.0f);  
	//glColor3f(0.0, 1.0, 0.0); // Green  
	//glVertex3f(-2.0f, -2.0f, 0.0f); 
	//glColor3f(0.0, 0.0, 1.0); // Blue   
	//glVertex3f(2.0f, -2.0f , 0.0f); 
	//glEnd();
	
	for (int i = 0; i < vnorms.size(); i++)
	{
		glBegin(GL_TRIANGLES);
		glNormal3f(vnorms[i]/5 , vnorms[i + 1]/5 , vnorms[i + 2]/5 );
		glVertex3f((verts[i]/5) , (verts[i + 1]/5 ) , (verts[i + 2]/5 ) );
		i += 3;
		glVertex3f((verts[i]/5 ) , (verts[i + 1]/5 ) , (verts[i + 2]/5 ) );
		i += 3;
		glVertex3f((verts[i]/5 ) , (verts[i + 1]/5 ) , (verts[i + 2]/5 ));
		i += 2;
		glEnd();
	}
	
	glFlush();
}



void CHXT1155132173View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
	if (nChar == VK_RIGHT)
		xtrans += 0.1;
	if (nChar == VK_LEFT)
		xtrans -= 0.1;
	if (nChar == VK_UP)
		ytrans += 0.1;
	if (nChar == VK_DOWN)
		ytrans -= 0.1;
	InvalidateRect(0, FALSE);


}


void CHXT1155132173View::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CView::OnMouseMove(nFlags, point);
	current_point = point;
	if (nFlags == MK_LBUTTON && current_point.y > last_point.y + 2)         
		xrot += 5.5;                                             
	else     
		if (nFlags == MK_LBUTTON && current_point.y < last_point.y - 2)       
			xrot -= 5.5;
	if (nFlags == MK_LBUTTON && current_point.x > last_point.x + 2)     
		yrot += 5.5;     
	else     
		if (nFlags == MK_LBUTTON && current_point.x < last_point.x - 2)    
			yrot -= 5.5;
	last_point = point;  //record the last point
	InvalidateRect(0,FALSE); 




}


BOOL CHXT1155132173View::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (zDelta > 0)   
		m_fScale += 0.1f;  
	else      
		if (m_fScale > 0.1f)   
			m_fScale -= 0.1f;
	InvalidateRect(NULL, FALSE);
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void CHXT1155132173View::OnFileOpen()
{
	// TODO: 在此添加命令处理程序代码
	CString szFilter;  
	szFilter = "stl|*.stl|*.*|*.*||"; 
	CFileDialog FD(TRUE, _T("stl"), _T("*.stl"), OFN_HIDEREADONLY, szFilter);
	string tmp;
	float a, b, c, d, e, f;
	if (FD.DoModal() == IDOK)
	{
		verts.clear();
		vnorms.clear();
		in.open(FD.GetPathName());
		while (!in.eof())
		{
			in >> tmp;	
			if (tmp.compare("normal") == 0)
			{
				in >> a >> b >> c;
				vnorms.push_back(a);
				vnorms.push_back(b);
				vnorms.push_back(c);
				vnorms.push_back(a);
				vnorms.push_back(b);
				vnorms.push_back(c);
				vnorms.push_back(a);
				vnorms.push_back(b);
				vnorms.push_back(c);
				in >> tmp;
				in >> tmp;
				in >> tmp;
				in >> d >> e >> f;
				verts.push_back(d);
				verts.push_back(e);
				verts.push_back(f);

				in >> tmp;
				in >> d >> e >> f;
				verts.push_back(d);
				verts.push_back(e);
				verts.push_back(f);

				in >> tmp;
				in >> d >> e >> f;
				verts.push_back(d);
				verts.push_back(e);
				verts.push_back(f);
				in >> tmp;
				in >> tmp;
			}

		}
								
		in.close();	
		
	}
	out.open("C:/Users/user/OneDrive/桌面/check.txt");
	for (int i = 0; i < verts.size(); i++)
	{
		out << verts[i] << " " << endl;
	}
	out.close();

}

void CHXT1155132173View::Lighting()
{
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE); // force the length of surface normal to be 1 so that the lighting and material effect can be calculated correctly
	glEnable(GL_DEPTH_TEST);
	// glEnable(GL_CULL_FACE);    // don't draw the hidden face

	glEnable(GL_LIGHT0);

	GLfloat light_position[] = { 1.0f, 0.0f, 1.0f, 0.0f }; //directional light
	GLfloat light_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat light_specular[] = { 1.0f,1.0f,1.0f,1.0f };

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
}

void CHXT1155132173View::Material()
{
	glEnable(GL_COLOR_MATERIAL);  ///Set the object's color with glColor3f();
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);  /// With using  glColor3f(), allow object's color to interact with light
	GLfloat mat_diffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat mat_specular[] = { 0.2f,0.2f,0.2f,1.0f };
	GLfloat mat_shininess[] = { 50.0f };
	GLfloat mat_emission[] = { 0.2f, 0.2f, 0.2, 1.0f };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
}

void CHXT1155132173View::Line()
{
	//clear background color of OpenGL scene 
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	//clear the screen and the depth buffer 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glShadeModel(GL_SMOOTH);
	glTranslatef(xtrans, ytrans, ztrans);
	glRotatef(xrot, 1, 0, 0);
	glRotatef(yrot, 0, 1, 0);
	glScalef(m_fScale, m_fScale, m_fScale);

	//glNormal3f(0.0, 0.0, 1.0);
 //   glBegin(GL_TRIANGLES); 
	//glColor3f(1.0, 0.0, 0.0); // Red  
	//glVertex3f(0.0f, 2.0f, 0.0f);  
	//glColor3f(0.0, 1.0, 0.0); // Green  
	//glVertex3f(-2.0f, -2.0f, 0.0f); 
	//glColor3f(0.0, 0.0, 1.0); // Blue   
	//glVertex3f(2.0f, -2.0f , 0.0f); 
	//glEnd();

	for (int i = 0; i < vnorms.size(); i++)
	{
		glBegin(GL_TRIANGLES);
		glNormal3f(vnorms[i] / 5, vnorms[i + 1] / 5, vnorms[i + 2] / 5);
		glVertex3f((verts[i] / 5), (verts[i + 1] / 5), (verts[i + 2] / 5));
		i += 3;
		glVertex3f((verts[i] / 5), (verts[i + 1] / 5), (verts[i + 2] / 5));
		i += 3;
		glVertex3f((verts[i] / 5), (verts[i + 1] / 5), (verts[i + 2] / 5));
		i += 2;
		glEnd();
	}

	glFlush();
}


void CHXT1155132173View::OnLineTransline()
{
	CDC* pDC = GetDC();
	wglMakeCurrent(pDC->m_hDC, m_hRC);
	glViewport(0, 0, Cx, Cy);
	SetupViewingFrustum((double)Cx / (double)Cy);
	SetupViewingTransform();
	Lighting();
	Material();
	Line();
	wglMakeCurrent(pDC->m_hDC, NULL);
	// TODO: 在此添加命令处理程序代码
}


void CHXT1155132173View::OnTestFill()
{
	CDC* pDC = GetDC();
	wglMakeCurrent(pDC->m_hDC, m_hRC);
	glViewport(0, 0, Cx, Cy);
	SetupViewingFrustum((double)Cx / (double)Cy);
	SetupViewingTransform();
	Lighting();
	Material();
	RenderScene();
	wglMakeCurrent(pDC->m_hDC, NULL);
	// TODO: 在此添加命令处理程序代码
}
