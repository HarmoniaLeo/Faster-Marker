
// marking.h: PROJECT_NAME 应用程序的主头文件
//
#include "afxdialogex.h"
#include <vector>
#include<opencv2/opencv.hpp>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include <math.h>
#include <algorithm>
using namespace cv;
using namespace std;
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// CmarkingApp:
// 有关此类的实现，请参阅 marking.cpp
//

class CmarkingApp : public CWinApp
{
public:
	CmarkingApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CmarkingApp theApp;

extern int markingId, firstX, firstY, moveTimes;
extern CString markingList[20];
extern bool clicked, firstPic;
extern CString folder, fileName;
extern Mat picTar, gryTar, picDis, idMat, oriPic;
extern const int color[20][3];
extern CBitmap m_backBitmap;
extern double times, range;