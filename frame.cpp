#include "stdafx.h"
#include "frame.h"

frame::frame(int x1, int y1, int x2, int y2, int _id, bool _dark, bool _difficult, bool _truncated, CString _pose)
{
	startX = x1;
	startY = y1;
	endX = x2;
	endY = y2;
	tag = markingId;
	id = _id;
	newFrame = 1;
	dark = _dark;
	difficult = _difficult;
	truncated = _truncated;
	pose = _pose;
	if (pose.IsEmpty())
		pose = "Unspecified";
}
/*
void frame::canel(int i,int j,Mat& diff,Mat& tarOri)
{
	for (int k = 0; k < 3; k++)
	{
		for (int l = 0; l < 3; l++)
		{
			if (tarOri.at<uchar>(i - 1 + k, j - 1 + l) == 255)
			{
				diff.at<uchar>(i, j) = 255;
				return;
			}
		}
	}
}*/

bool frame::action()
{
	Mat tarOri = gryTar.clone()(Rect(startX, startY, endX - startX, endY - startY));
	double** posMat = new double*[gryTar.rows];
	for (int i = 0; i < gryTar.rows; i++)
		posMat[i] = new double[gryTar.cols];
	toOne(tarOri, dark, posMat);
	if (newFrame)
	{
		eXp = eXCal(posMat, tarOri.cols, tarOri.rows);
		eYp = eYCal(posMat, tarOri.cols, tarOri.rows);
		sigmaX = sigmaXCal(posMat, tarOri.cols, tarOri.rows);
		sigmaY = sigmaYCal(posMat, tarOri.cols, tarOri.rows);
		newFrame = 0;
	}
	else
	{
		int i = 0;
		for (; i < moveTimes; i++)
		{
			double eXnew = eXCal(posMat, tarOri.cols, tarOri.rows), eYnew = eYCal(posMat, tarOri.cols, tarOri.rows);
			moving(eXnew, eYnew);
			tarOri = gryTar.clone()(Rect(startX, startY, endX - startX, endY - startY));
			toOne(tarOri, dark, posMat);
			eXnew = eXCal(posMat, tarOri.cols, tarOri.rows), eYnew = eYCal(posMat, tarOri.cols, tarOri.rows);
			double sigmaXnew = sigmaXCal(posMat, tarOri.cols, tarOri.rows), sigmaYnew = sigmaYCal(posMat, tarOri.cols, tarOri.rows);
			expanding(sigmaXnew, sigmaYnew);
			tarOri = gryTar.clone()(Rect(startX, startY, endX - startX, endY - startY));
			toOne(tarOri, dark, posMat);
			eXnew = eXCal(posMat, tarOri.cols, tarOri.rows), eYnew = eYCal(posMat, tarOri.cols, tarOri.rows);
			sigmaXnew = sigmaXCal(posMat, tarOri.cols, tarOri.rows), sigmaYnew = sigmaYCal(posMat, tarOri.cols, tarOri.rows);
			if (abs(eXnew - eXp) < 0.3*range &&abs(eYnew - eYp) < 0.3*range &&abs(sigmaXnew - sigmaX) < range &&abs(sigmaYnew - sigmaY) < range)
			{
				eXp = eXnew;
				eYp = eYnew;
				sigmaX = sigmaXnew;
				sigmaY = sigmaYnew;
				break;
			}
		}
		if (i == moveTimes || (startX == endX) || (startY == endY))
		{
			for (int i = 0; i < gryTar.rows; i++)
				delete[] posMat[i];
			delete[] posMat;
			return false;
		}
	}
	for (int i = 0; i < gryTar.rows; i++)
		delete[] posMat[i];
	delete[] posMat;
	return true;
}

void frame::toOne(Mat& tarOri, bool dark, double** res)
{
	long long sum = 0;
	if (dark)
	{
		Mat tarOri2 = tarOri.clone();
		for (int i = 0; i < tarOri.cols; i++)
			for (int j = 0; j < tarOri.rows; j++)
				tarOri2.at<uchar>(j, i) = 255 - tarOri2.at<uchar>(j, i);
		for (int i = 0; i < tarOri.cols; i++)
			for (int j = 0; j < tarOri.rows; j++)
				sum += tarOri2.at<uchar>(j, i);
		for (int i = 0; i < tarOri.cols; i++)
			for (int j = 0; j < tarOri.rows; j++)
				res[j][i] = (double)tarOri2.at<uchar>(j, i) / sum;
	}
	for (int i = 0; i < tarOri.cols; i++)
		for (int j = 0; j < tarOri.rows; j++)
			sum += tarOri.at<uchar>(j, i);
	for (int i = 0; i < tarOri.cols; i++)
		for (int j = 0; j < tarOri.rows; j++)
			res[j][i] = (double)tarOri.at<uchar>(j, i) / sum;
}

double frame::eXCal(double** tarOri, int lengthX, int lengthY)
{
	double* fx;
	fx = new double[lengthX];
	for (int i = 0; i < lengthX; i++)
		fx[i] = 0;
	for (int i = 0; i < lengthX; i++)
		for (int j = 0; j < lengthY; j++)
			fx[i] += tarOri[j][i];
	for (int i = 1; i < lengthX; i++)
	{
		fx[i] += fx[i - 1];
		if (fx[i] > 0.5)
			return (double)i / lengthX;
	}
	delete[] fx;
	return 0.5;
}

double frame::eYCal(double** tarOri, int lengthX, int lengthY)
{
	double* fx;
	fx = new double[lengthY];
	for (int i = 0; i < lengthY; i++)
		fx[i] = 0;
	for (int j = 0; j < lengthY; j++)
		for (int i = 0; i < lengthX; i++)
			fx[j] += tarOri[j][i];
	for (int i = 1; i < lengthY; i++)
	{
		fx[i] += fx[i - 1];
		if (fx[i] > 0.5)
			return (double)i / lengthY;
	}
	delete[] fx;
	return 0.5;
}

void frame::moving(double newX, double newY)
{
	//	int perX = (2 - sigmaX) > 1 ? (2 - sigmaX) : 1, perY = (2 - sigmaY) > 1 ? (2 - sigmaY) : 1;
	int moveX = newX * (endX - startX) - eXp * (endX - startX), moveY = newY * (endY - startY) - eYp * (endY - startY);
	if (startX + moveX >= 0)
	{
		if (startX + moveX < gryTar.cols)
			startX = startX + moveX;
		else
			startX = gryTar.cols - 1;
	}
	else
		startX = 0;
	if (endX + moveX >= 0)
	{
		if (endX + moveX < gryTar.cols)
			endX = endX + moveX;
		else
			endX = gryTar.cols - 1;
	}
	else
		endX = 0;
	if (startY + moveY >= 0)
	{
		if (startY + moveY < gryTar.rows)
			startY = startY + moveY;
		else
			startY = gryTar.rows - 1;
	}
	else
		startY = 0;
	if (endY + moveY >= 0)
	{
		if (endY + moveY < gryTar.rows)
			endY = endY + moveY;
		else
			endY = gryTar.rows - 1;
	}
	else
		endX = 0;
}

double frame::sigmaXCal(double** tarOri, int lengthX, int lengthY)
{
	double* fx;
	fx = new double[lengthX];
	for (int i = 0; i < lengthX; i++)
		fx[i] = 0;
	for (int i = 0; i < lengthX; i++)
		for (int j = 0; j < lengthY; j++)
			fx[i] += tarOri[j][i];
	double s2 = 0;
	for (int i = 0; i < lengthX; i++)
		s2 += ((double)fx[i] - (double)1 / lengthX)*((double)fx[i] - (double)1 / lengthX);
	s2 /= lengthX - 1;
	delete[] fx;
	return sqrt(s2);
}

double frame::sigmaYCal(double** tarOri, int lengthX, int lengthY)
{
	double* fx;
	fx = new double[lengthY];
	for (int i = 0; i < lengthY; i++)
		fx[i] = 0;
	for (int j = 0; j < lengthY; j++)
		for (int i = 0; i < lengthX; i++)
			fx[j] += tarOri[j][i];
	double s2 = 0;
	for (int i = 0; i < lengthY; i++)
		s2 += ((double)fx[i] - (double)1 / lengthY)*((double)fx[i] - (double)1 / lengthY);
	s2 /= lengthY - 1;
	delete[] fx;
	return sqrt(s2);
}

void frame::expanding(double sX, double sY)
{
	double perX1 = sigmaX / sX, perY1 = sigmaY / sY;
	double perX = abs(perX1 - 1) < abs(perY1 - 1) ? perX1 : perY1;
	double perY = abs(perX1 - 1) < abs(perY1 - 1) ? perX1 : perY1;
	if ((startX + endX) / 2 + (startX - (startX + endX) / 2)*perX >= 0)
	{
		if (startX + (startX - (startX + endX) / 2)*perX < gryTar.cols)
			startX = (startX + endX) / 2 + (startX - (startX + endX) / 2)*perX;
		else
			startX = gryTar.cols - 1;
	}
	else
		startX = 0;
	if ((startX + endX) / 2 + (endX - (startX + endX) / 2)*perX >= 0)
	{
		if ((startX + endX) / 2 + (endX - (startX + endX) / 2)*perX < gryTar.cols)
			endX = (startX + endX) / 2 + (endX - (startX + endX) / 2)*perX;
		else
			endX = gryTar.cols - 1;
	}
	else
		endX = 0;
	if ((startY + endY) / 2 + (startY - (startY + endY) / 2)*perY >= 0)
	{
		if ((startY + endY) / 2 + (startY - (startY + endY) / 2)*perY < gryTar.rows)
			startY = (startY + endY) / 2 + (startY - (startY + endY) / 2)*perY;
		else
			startY = gryTar.rows - 1;
	}
	else
		startY = 0;
	if ((startY + endY) / 2 + (endY - (startY + endY) / 2)*perY >= 0)
	{
		if ((startY + endY) / 2 + (endY - (startY + endY) / 2)*perY < gryTar.rows)
			endY = (startY + endY) / 2 + (endY - (startY + endY) / 2)*perY;
		else
			endY = gryTar.rows - 1;
	}
	else
		endY = 0;
}

/*
typedef struct
{
	int y, x;
}pit;

void frame::bfs(Mat& diff,Mat& tarOri)
{
	Mat searched= Mat(diff.rows, diff.cols, CV_8UC(1), Scalar::all(0));
	for (int j = 0; j < diff.rows; j++)
	{
		for (int i = 0; i < diff.cols; i++)
		{
			if (diff.at<uchar>(j,i) == 0 && searched.at<uchar>(j,i) == 0)
			{
				BLOCK blc{0,0,0,0};
				vector<pit> pits;
				pits.push_back(pit{ j, i });
				searched.at<uchar>(j, i) = 1;
				for (int k = 0; k < pits.size(); k++)
				{
					blc.size += 1;
					blc.level +=tarOri.at<uchar>(pits[k].y,pits[k].x);
					blc.x +=pits[k].x;
					blc.y +=pits[k].y;
					if (pits[k].x + 1 < diff.cols&&searched.at<uchar>(pits[k].y, pits[k].x + 1) == 0&& diff.at<uchar>(pits[k].y, pits[k].x + 1) == 0)
					{
						pits.push_back(pit{ pits[k].y, pits[k].x + 1 });
						searched.at<uchar>(pits[k].y, pits[k].x + 1) = 1;
					}
					if (pits[k].y + 1 < diff.rows&&searched.at<uchar>(pits[k].y+1, pits[k].x) == 0&& diff.at<uchar>(pits[k].y + 1, pits[k].x) == 0)
					{
						pits.push_back(pit{ pits[k].y+1, pits[k].x });
						searched.at<uchar>(pits[k].y+1, pits[k].x) = 1;
					}
					if (pits[k].x - 1 >=0&&searched.at<uchar>(pits[k].y, pits[k].x - 1) == 0&& diff.at<uchar>(pits[k].y, pits[k].x - 1) == 0)
					{
						pits.push_back(pit{ pits[k].y, pits[k].x - 1 });
						searched.at<uchar>(pits[k].y, pits[k].x - 1) = 1;
					}
					if (pits[k].y - 1 >=0&&searched.at<uchar>(pits[k].y-1, pits[k].x) == 0&& diff.at<uchar>(pits[k].y - 1, pits[k].x) == 0)
					{
						pits.push_back(pit{ pits[k].y-1, pits[k].x });
						searched.at<uchar>(pits[k].y-1, pits[k].x) = 1;
					}
				}
				if ((double)blc.size / (diff.rows*diff.cols) < 0.0025)
					continue;
				blc.level /= blc.size;
				blc.x /= blc.size;
				blc.y /= blc.size;
				if((int)blc.level / 5 - 1>=0)
					thisOne[(int)(blc.level/5-1)].push_back(blc);
				else
					thisOne[0].push_back(blc);
			}
		}
	}
}

void frame::quickSort(int* edge, int* posDiff, int start,int end)
{
	if (start >= end)
		return;
	int base = posDiff[start],ebase=edge[start],i=start,j=end;
	while (i < j)
	{
		while (i < j)
		{
			if (posDiff[j] < base)
			{
				posDiff[i] = posDiff[j];
				edge[i] = edge[j];
				j--;
				break;
			}
			j--;
		}
		while (i < j)
		{
			if (posDiff[i] > base)
			{
				posDiff[j] = posDiff[i];
				edge[j] = edge[i];
				i++;
				break;
			}
			i++;
		}
	}
	posDiff[i] = base;
	edge[i] = ebase;
	quickSort(edge, posDiff, start, i - 1);
	quickSort(edge, posDiff, i + 1, end);
}

int frame::dist(int x1, int x2, int y1, int y2)
{
	return (int)(sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2)));
}

DIFF frame::dinic(vector<BLOCK>& thisOne, vector<BLOCK>& lastOne)
{
	DIFF res{ 0,0,0,0 };
	if (lastOne.size() == 0 || thisOne.size() == 0)
		return res;
	int **edge,*edgeRec,*edgeBac;
	vector<int> start;
	vector<double> median;
	edgeBac = new int[lastOne.size()];
	edge = new int*[thisOne.size()];
	edgeRec = new int[thisOne.size()];
	for (int i = 0; i < thisOne.size(); i++)
	{
		edge[i] = new int[lastOne.size()];
		edgeRec[i] = -1;
		start.push_back(i);
	}
	for (int i = 0; i < lastOne.size(); i++)
		edgeBac[i] = -1;

	for (int i = 0; i < thisOne.size(); i++)
	{
		int* posDiff;
		posDiff = new int[lastOne.size()];
		for (int j = 0; j < lastOne.size(); j++)
		{
			edge[i][j] = j;
			posDiff[j] = dist(thisOne[i].x,lastOne[j].x,thisOne[i].y,lastOne[j].y);
		}
		quickSort(edge[i], posDiff, 0, lastOne.size() - 1);
		delete[] posDiff;
	}
	for (int i = 0; i < start.size(); i++)
	{
		for (int j = edgeRec[start[i]]+1; j < lastOne.size(); j++)
		{
			if (edgeBac[edge[start[i]][j]] >= 0)
			{
				if (dist(thisOne[start[i]].x, lastOne[edge[start[i]][j]].x, thisOne[start[i]].y, lastOne[edge[start[i]][j]].y) < dist(thisOne[edgeBac[edge[start[i]][j]]].x, lastOne[edge[start[i]][j]].x, thisOne[edgeBac[edge[start[i]][j]]].y, lastOne[edge[start[i]][j]].y))
				{
					start.push_back(edgeBac[edge[start[i]][j]]);
					edgeBac[edge[start[i]][j]] = start[i];
					edgeRec[start[i]] = j;
					break;
				}
			}
			else
			{
				edgeBac[edge[start[i]][j]] = start[i];
				edgeRec[start[i]] = j;
				break;
			}
		}
	}
	for (int i = 0; i < lastOne.size(); i++)
		if (edgeBac[i] >= 0)
			median.push_back((double)thisOne[edgeBac[i]].size / lastOne[i].size);
	int matched = 0;
	if (median.size() != 0)
	{
		sort(median.begin(), median.end());
		double medianValue = median[(int)median.size() / 2];
		for (int i = 0; i < lastOne.size(); i++)
		{
			if (edgeBac[i] >= 0&&abs(medianValue- (double)thisOne[edgeBac[i]].size / lastOne[i].size)<0.2)
			{
				res.per += (double)thisOne[edgeBac[i]].size / lastOne[i].size;
				res.x += thisOne[edgeBac[i]].x - lastOne[i].x;
				res.y += thisOne[edgeBac[i]].y - lastOne[i].y;
				matched++;
			}

		}
		if (!matched == 0)
		{
			res.x /= matched;
			res.y /= matched;
			res.per /= matched;
		}
	}
	if ((double)matched * 2 / (lastOne.size() + thisOne.size()) <= 0.5)
		res.matched = 0;
	else
		res.matched = matched;
	for (int i = 0; i < thisOne.size(); i++)
		delete[] edge[i];
	delete [] edgeRec,edgeBac,edge;
	return res;
}

DIFF frame::match()
{
	DIFF res{0,0,0,0},amatch;
	int matched=0,groups=0;
	vector<double> median;
	vector<DIFF> ress;
	for (int i = 0; i < 51; i++)
	{
		amatch = dinic(thisOne[i], lastOne[i]);
		groups += thisOne[i].size()+lastOne[i].size();
		if (amatch.matched == 0)
			continue;
		else
		{
			ress.push_back(amatch);
			median.push_back(amatch.per);
		}
	}
	if (median.size() != 0)
	{
		sort(median.begin(), median.end());
		double medianValue = median[(int)median.size() / 2];
		for (int i = 0; i < ress.size(); i++)
		{
			if (abs(medianValue - ress[i].per) < 0.2)
			{
				res.per += ress[i].per;
				res.x += ress[i].x;
				res.y += ress[i].y;
				matched+=ress[i].matched;
			}
		}
		if (!matched == 0)
		{
			res.x /= matched;
			res.y /= matched;
			res.per /= matched;
		}
	}
	if((double)matched*2/groups<0.1)
		res.matched = 0;
	else
		res.matched = matched;
	return res;
}*/

void frame::paint()
{
	Vec3b pixel;
	pixel[0] = color[tag][0];
	pixel[1] = color[tag][1];
	pixel[2] = color[tag][2];
	for (int i = startX; i < endX; i++)
	{
		picDis.at<cv::Vec3b>(startY, i) = pixel;
		idMat.at<uchar>(startY, i) = id;
	}
	for (int i = startX; i < endX; i++)
	{
		picDis.at<Vec3b>(endY, i) = pixel;
		idMat.at<uchar>(endY, i) = id;
	}
	for (int i = startY; i < endY; i++)
	{
		picDis.at<Vec3b>(i, startX) = pixel;
		idMat.at<uchar>(i, startX) = id;
	}
	for (int i = startY; i < endY; i++)
	{
		picDis.at<Vec3b>(i, endX) = pixel;
		idMat.at<uchar>(i, endX) = id;
	}
}

void frame::write(CStdioFile& xml)
{
	xml.WriteString("<object><name>" + markingList[tag] + "</name>");
	xml.WriteString("<pose>" + pose + "</pose>");
	if (truncated)
		xml.WriteString("<truncated>1</truncated>");
	else
		xml.WriteString("<truncated>0</truncated>");
	if (difficult)
		xml.WriteString("<difficult>1</difficult>");
	else
		xml.WriteString("<difficult>0</difficult>");
	CString buf;
	buf.Format("%d", (int)(startX*times));
	xml.WriteString("<bndbox><xmin>" + buf + "</xmin>");
	buf.Format("%d", (int)(startY*times));
	xml.WriteString("<ymin>" + buf + "</ymin>");
	buf.Format("%d", (int)(endX*times));
	xml.WriteString("<xmax>" + buf + "</xmax>");
	buf.Format("%d", (int)(endY*times));
	xml.WriteString("<ymax>" + buf + "</ymax></bndbox></object>");
}