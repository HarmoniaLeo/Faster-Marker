#include "marking.h"
#include "frame.h"

#pragma once
class frameList
{
private:
	vector<frame*> list;
	int i;
public:
	frameList() { i = 0; }
	void add(int, int, int, int, bool, bool, bool, CString);
	void action();
	void paint();
	void del(int);
	void write(CStdioFile&);
};

extern frameList flist;