
// markingDlg.h: 头文件
//

#include "marking.h"
#pragma once



// CmarkingDlg 对话框
class CmarkingDlg : public CDialogEx
{
// 构造
public:
	CmarkingDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MARKING_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton2();
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton3();
//	afx_msg void OnClickedBitmap();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
private:
	int rePaint();
public:
	afx_msg void OnStnClickedStatic6();
	afx_msg void OnNMCustomdrawSlider2(NMHDR *pNMHDR, LRESULT *pResult);
private:
	int save();
public:
	afx_msg void OnDestroy();
	afx_msg void OnClickedButton2();
	CSliderCtrl sliderMove;
	CSliderCtrl sliderTimes;
	CSliderCtrl sliderRange;
	afx_msg void OnStnClickedStatic10();
	afx_msg void OnReleasedcaptureSlider3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnReleasedcaptureSlider4(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnClickedButton5();
//	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnNMCustomdrawSlider3(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnAccelerator32772();
//	afx_msg void OnAccelerator32771();
//	afx_msg void OnAccelerator32773();
//	afx_msg void OnAccelerator32774();
//	afx_msg void OnAccelerator32775();
//	afx_msg void OnAccelerator32776();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

