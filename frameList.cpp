#include "stdafx.h"
#include "frameList.h"

void frameList::add(int x1, int y1, int x2, int y2, bool dark, bool truncated, bool difficult, CString pose)
{
	if (i == list.size())
	{
		list.push_back(new frame(x1, y1, x2, y2, i, dark, truncated, difficult, pose));
		list[i]->action();
		i++;
	}
	else
	{
		list[i] = new frame(x1, y1, x2, y2, i, dark, truncated, difficult, pose);
		list[i]->action();
		for (; i < list.size(); i++)
		{
			if (list[i] == nullptr)
				break;
		}
	}
}

void frameList::action()
{
	for (int j = 0; j < list.size(); j++)
	{
		if (list[j] == nullptr)
			continue;
		if (!list[j]->action())
		{
			delete list[j];
			list[j] = nullptr;
			if (i > j)
				i = j;
		}
	}
}

void frameList::paint()
{
	picDis = picTar.clone();

	idMat = Mat(picDis.rows, picDis.cols, CV_8UC(1), Scalar::all(20));
	for (int j = 0; j < list.size(); j++)
	{
		if (list[j] != nullptr)
			list[j]->paint();
	}
}

void frameList::del(int id)
{
	if (list[id] != nullptr)
		delete list[id];
	list[id] = nullptr;
	if (i > id)
		i = id;
}

void frameList::write(CStdioFile& xml)
{
	for (int i = 0; i < list.size(); i++)
	{
		if (list[i] != nullptr)
			list[i]->write(xml);
	}
}
