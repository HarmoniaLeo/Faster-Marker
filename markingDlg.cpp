
// markingDlg.cpp: 实现文件
//
#include "stdafx.h"
#include "markingDlg.h"
#include "frame.h"
#include "frameList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// 用于应用程序“关于”菜单项的 CAboutDlg 对话框


//全局变量

int markingId,firstX,firstY,moveTimes;
CString markingList[20];
bool clicked,firstPic;
frameList flist;
CString folder,fileName;
Mat picTar, gryTar,picDis,idMat,oriPic;
const int color[20][3] = { {0,0,255},{0,127,127}, {0,127,255},{0,255,0},{0,255,127},{0,255,255},{127,0,127},{127,0,255},{127,127,0},{127,127,255},{127,255,0},{127,255,127},{127,255,255},{255,0,0},{255,0,127},{255,0,255},{255,127,0},{255,127,255},{255,255,0},{255,255,127} };
CBitmap m_backBitmap;
double times,range;
HACCEL m_hAcc;

//框架部分


class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CmarkingDlg 对话框



CmarkingDlg::CmarkingDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MARKING_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CmarkingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
//	DDX_Control(pDX, IDC_SLIDER2, sliderMove);
	DDX_Control(pDX, IDC_SLIDER3, sliderTimes);
	DDX_Control(pDX, IDC_SLIDER4, sliderRange);
}

BEGIN_MESSAGE_MAP(CmarkingDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON4, &CmarkingDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON3, &CmarkingDlg::OnBnClickedButton3)
//	ON_STN_CLICKED(IDC_Bitmap, &CmarkingDlg::OnClickedBitmap)
	ON_BN_CLICKED(IDC_BUTTON1, &CmarkingDlg::OnBnClickedButton1)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_STN_CLICKED(IDC_STATIC6, &CmarkingDlg::OnStnClickedStatic6)
//	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER2, &CmarkingDlg::OnNMCustomdrawSlider2)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON2, &CmarkingDlg::OnClickedButton2)
	ON_STN_CLICKED(IDC_STATIC10, &CmarkingDlg::OnStnClickedStatic10)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER3, &CmarkingDlg::OnReleasedcaptureSlider3)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER4, &CmarkingDlg::OnReleasedcaptureSlider4)
	ON_WM_KEYDOWN()
	ON_BN_CLICKED(IDC_BUTTON5, &CmarkingDlg::OnClickedButton5)
	ON_WM_KEYUP()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER3, &CmarkingDlg::OnNMCustomdrawSlider3)
//	ON_COMMAND(ID_ACCELERATOR32772, &CmarkingDlg::OnAccelerator32772)
//	ON_COMMAND(ID_ACCELERATOR32771, &CmarkingDlg::OnAccelerator32771)
//	ON_COMMAND(ID_ACCELERATOR32773, &CmarkingDlg::OnAccelerator32773)
//	ON_COMMAND(ID_ACCELERATOR32774, &CmarkingDlg::OnAccelerator32774)
//	ON_COMMAND(ID_ACCELERATOR32775, &CmarkingDlg::OnAccelerator32775)
//	ON_COMMAND(ID_ACCELERATOR32776, &CmarkingDlg::OnAccelerator32776)
END_MESSAGE_MAP()


// CmarkingDlg 消息处理程序

BOOL CmarkingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	markingId = 0;
	clicked = 0;
	firstPic = 1;
	CoInitialize(NULL);

	sliderTimes.SetRange(1, 30);
	sliderTimes.SetTicFreq(5);
	sliderRange.SetRange(1, 10);
	sliderTimes.SetTicFreq(1);

	HACCEL m_hAcc = LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_ACCELERATOR1));

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CmarkingDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CmarkingDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CmarkingDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



//消息部分


void CmarkingDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	if (markingId > 0)
	{
		CString buf;
		GetDlgItemText(IDC_EDIT1, buf);
		markingList[markingId] = buf;
		markingId--;
		SetDlgItemText(IDC_EDIT1, markingList[markingId]);
	}
}


void CmarkingDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	if (markingId < 19)
	{
		CString buf;
		GetDlgItemText(IDC_EDIT1, buf);
		markingList[markingId] = buf;
		markingId++;
		SetDlgItemText(IDC_EDIT1, markingList[markingId]);
	}
}


//void CmarkingDlg::OnClickedBitmap()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	
//}


CFileFind findFile;

void CmarkingDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	save();
	CString buf;
	GetDlgItemText(IDC_EDIT2, buf);
	if (buf == "")
	{
		MessageBox("请先设置文件夹");
		return;
	}
	CString strFileTemp, strFileName;
	int nIsFind = 1, count = 0;
	if (buf != folder)
	{
		folder = buf;
		nIsFind = findFile.FindFile(folder + "\\*");
	}

	while(nIsFind)
	{
		nIsFind = findFile.FindNextFile();
		CString fulldir = folder + "\\" + findFile.GetFileName();
		fileName = findFile.GetFileName();
		string dir = CT2A(fulldir.GetBuffer(fulldir.GetLength()));
		picTar = imread(dir);
		oriPic = picTar.clone();
		if (picTar.data != NULL)
		{
			SetDlgItemText(IDD_MARKING_DIALOG, findFile.GetFileName());
			gryTar = imread(dir, CV_8UC(1));
			double timesx = (double)gryTar.cols / 1080;
			double timesy = (double)gryTar.rows / 960;
			times = timesx > timesy ? timesx : timesy;
			resize(gryTar, gryTar, Size((int)(gryTar.cols / times), (int)(gryTar.rows / times)));
			resize(picTar, picTar, Size((int)(picTar.cols / times), (int)(picTar.rows / times)));

			flist.action();
			rePaint();

			break;
		}
		count++;
	}
	if (count == nIsFind)
	{
		MessageBox("最后一张了");
		return;
	}
	if(nIsFind==0)
	{
		MessageBox("目录无效");
		return;
	}
}


void CmarkingDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnLButtonDown(nFlags, point);

	POINT pt;
	GetCursorPos(&pt);
	CStatic *item = (CStatic *)GetDlgItem(IDC_STATIC);
	item->ScreenToClient(&pt);
	if (pt.x > picDis.cols || pt.x<0 || pt.y>picDis.rows || pt.y < 0)
		return;
	if (!clicked)
	{
		firstX = pt.x;
		firstY = pt.y;
		clicked = 1;
	}
	else
	{
		clicked = 0;
		CString buf;
		GetDlgItemText(IDC_EDIT1, buf);
		if (buf == "")
		{
			MessageBox("请先设置标签名");
			return;
		}
		if(pt.x<firstX||pt.y < firstY)
		{
			MessageBox("第二个点需要在第一个点的右下角");
			return;
		}
		GetDlgItemText(IDC_EDIT4, buf);
		flist.add(firstX, firstY, pt.x, pt.y, ((CButton *)GetDlgItem(IDC_CHECK1))->GetCheck(), ((CButton *)GetDlgItem(IDC_CHECK2))->GetCheck(), ((CButton *)GetDlgItem(IDC_CHECK3))->GetCheck(),buf);
		rePaint();
	}
}


void CmarkingDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnRButtonDown(nFlags, point);

	POINT pt;
	GetCursorPos(&pt);
	CStatic *item = (CStatic *)GetDlgItem(IDC_STATIC);
	item->ScreenToClient(&pt);
	if (pt.x > picDis.cols || pt.x<0 || pt.y>picDis.rows || pt.y < 0)
		return;
	for (int i = 0; i < 10; i++)
	{
		if (pt.x + i < idMat.cols&&idMat.at<uchar>(pt.y, pt.x + i) != 20)
		{
			flist.del(idMat.at<uchar>(pt.y, pt.x + i));
			rePaint();
			return;
		}
		if (pt.y + i < idMat.rows&&idMat.at<uchar>(pt.y + i, pt.x) != 20)
		{
			flist.del(idMat.at<uchar>(pt.y + i, pt.x));
			rePaint();
			return;
		}
	}
	for (int i = -1; i>-10; i--)
	{
		if (pt.x + i >= 0&&idMat.at<uchar>(pt.y, pt.x + i) != 20)
		{
			flist.del(idMat.at<uchar>(pt.y, pt.x + i));
			rePaint();
			return;
		}
		if (pt.y + i >= 0&&idMat.at<uchar>(pt.y + i, pt.x) != 20)
		{
			flist.del(idMat.at<uchar>(pt.y + i, pt.x));
			rePaint();
			return;
		}
	}
	
}


int CmarkingDlg::rePaint()
{
	// TODO: 在此处添加实现代码.
	flist.paint();
	imwrite("tmp.bmp", picDis);

	CStatic *p = (CStatic *)GetDlgItem(IDC_STATIC);
	p->ModifyStyle(0, SS_BITMAP);
	p->SetBitmap((HBITMAP)::LoadImage(NULL, _T("tmp.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));
	p->SetWindowPos(NULL, 400, 0, 0, 0, SWP_NOZORDER | SWP_NOSIZE);

	CRect rectDlg;
	GetWindowRect(rectDlg);//获得窗体在屏幕上的位置

	CRect rectCtrl;
	p->GetWindowRect(rectCtrl);
	this->ScreenToClient(rectCtrl);
	MoveWindow(rectDlg.left, rectDlg.top, rectCtrl.Width() + 400, 1180);
	return 0;
}


void CmarkingDlg::OnStnClickedStatic6()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CmarkingDlg::OnNMCustomdrawSlider2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}


int CmarkingDlg::save()
{
	// TODO: 在此处添加实现代码.
	if (!firstPic)
	{
		CStdioFile xml;
		CString buf, buf2;
		GetDlgItemText(IDC_EDIT3, buf);
		if (!xml.Open(buf + "\\" + fileName.Left(fileName.ReverseFind('.')) + ".xml", CFile::modeCreate | CFile::modeWrite))
		{
			MessageBox("创建或修改标签失败");
			return 0;
		}
		xml.WriteString("<annotation>");
		xml.WriteString("<folder>" + buf.Left(fileName.ReverseFind('\\') - 1) + "</folder>");
		xml.WriteString("<filename>" + fileName + "</filename>");
		xml.WriteString("<size><width>");
		buf2.Format("%d", oriPic.rows);
		xml.WriteString(buf2 + "</width><height>");
		buf2.Format("%d", oriPic.cols);
		xml.WriteString(buf2 + "</height><depth>");
		buf2.Format("%d", oriPic.channels());
		xml.WriteString(buf2 + "</depth></size>");
		xml.WriteString("<segmented>1</segmented>");
		flist.write(xml);
		xml.WriteString("</annotation>");
		xml.Close();
	}
	else
		firstPic = false;
	return 0;
}


void CmarkingDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	CoUninitialize();
}


void CmarkingDlg::OnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CString buf;
	GetDlgItemText(IDC_EDIT2, buf);
	if (buf == "")
	{
		MessageBox("请先设置文件夹");
		return;
	}
	CString strFileTemp, strFileName;
	int nIsFind = 1;
	if (buf != folder)
	{
		folder = buf;
		nIsFind = findFile.FindFile(folder + "\\*");
	}

	while (nIsFind)
	{
		nIsFind = findFile.FindNextFile();
		CString fulldir = folder + "\\" + findFile.GetFileName();
		fileName = findFile.GetFileName();
		string dir = CT2A(fulldir.GetBuffer(fulldir.GetLength()));
		picTar = imread(dir);
		oriPic = picTar.clone();
		if (picTar.data != NULL)
		{
			SetDlgItemText(IDD_MARKING_DIALOG, findFile.GetFileName());
			gryTar = imread(dir, CV_8UC(1));
			double timesx = 1,timesy=1;
			if(gryTar.cols>1080)
				timesx= (double)gryTar.cols / 1080;
			if (gryTar.cols > 960)
				timesy = (double)gryTar.rows / 960;
			times = timesx > timesy ? timesx : timesy;
			resize(gryTar, gryTar, Size((int)(gryTar.cols / times), (int)(gryTar.rows / times)));
			resize(picTar, picTar, Size((int)(picTar.cols / times), (int)(picTar.rows / times)));

			flist.action();
			rePaint();

			break;
		}
	}
}


void CmarkingDlg::OnStnClickedStatic10()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CmarkingDlg::OnReleasedcaptureSlider3(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	moveTimes=sliderTimes.GetPos();
}


void CmarkingDlg::OnReleasedcaptureSlider4(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	range=0.02*sliderRange.GetPos();
}



void CmarkingDlg::OnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	for (int i = 0; i < 20; i++)
		flist.del(i);
	rePaint();
}



void CmarkingDlg::OnNMCustomdrawSlider3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

//void CmarkingDlg::OnAccelerator32772()
//{
//	// TODO: 在此添加命令处理程序代码
//	OnBnClickedButton1();
//}


//void CmarkingDlg::OnAccelerator32771()
//{
//	// TODO: 在此添加命令处理程序代码
//	OnClickedButton2();
//}


//void CmarkingDlg::OnAccelerator32773()
//{
//	// TODO: 在此添加命令处理程序代码
//	OnBnClickedButton4();
//}


//void CmarkingDlg::OnAccelerator32774()
//{
//	// TODO: 在此添加命令处理程序代码
//	OnBnClickedButton3();
//}


//void CmarkingDlg::OnAccelerator32775()
//{
//	// TODO: 在此添加命令处理程序代码
//	OnClickedButton5();
//}


//void CmarkingDlg::OnAccelerator32776()
//{
//	// TODO: 在此添加命令处理程序代码
//	((CButton *)GetDlgItem(IDC_CHECK1))->SetCheck(!((CButton *)GetDlgItem(IDC_CHECK1))->GetCheck());
//}


BOOL CmarkingDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN&&GetFocus()!=GetDlgItem(IDC_EDIT1)&& GetFocus() != GetDlgItem(IDC_EDIT2) && GetFocus() != GetDlgItem(IDC_EDIT3) && GetFocus() != GetDlgItem(IDC_EDIT4))
	{
		int nkeyDown = pMsg->wParam;
		if(nkeyDown=='S')
			OnBnClickedButton1();
		else if(nkeyDown=='W')
			OnClickedButton2();
		else if (nkeyDown == 'A')
			OnBnClickedButton4();
		else if (nkeyDown == 'D')
			OnBnClickedButton3();
		else if (nkeyDown == 'Q')
			OnClickedButton5();
		else if (nkeyDown == 'E')
			((CButton *)GetDlgItem(IDC_CHECK1))->SetCheck(!((CButton *)GetDlgItem(IDC_CHECK1))->GetCheck());
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
