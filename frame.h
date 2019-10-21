#include "marking.h"
#pragma once


class frame
{
private:
	int startX, startY, endX, endY, id, tag;
	double eXp, eYp;
	bool newFrame;
	double sigmaX, sigmaY;
	void toOne(Mat&, bool, double**);
	double eXCal(double**, int, int);
	double eYCal(double**, int, int);
	void moving(double, double);
	double sigmaXCal(double**, int, int);
	double sigmaYCal(double**, int, int);
	void expanding(double, double);
	bool dark, truncated, difficult;
	CString pose;
public:
	frame(int, int, int, int, int, bool, bool, bool, CString);
	bool action();
	void paint();
	void write(CStdioFile&);
};