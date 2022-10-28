#include "mex.h"
#include <iostream>
#include <queue>
#include <stack>
#include <string>
using namespace std;

void colFirstToRowFirst(double*data,int M,int N){
	double* t = new double[M * N]{};
	for (int col = 0; col < N; col++) {
		for (int row = 0; row < M; row++) {
			t[row * N + col] = data[col * M + row];
		}
	}
	for (int i = 0; i < M * N; i++) {
		data[i] = t[i];
	}
	delete[] t;



}
void rowFirstToColFirst(double* data, int M, int N) {
	double* t = new double[M * N]{};
	for (int row = 0; row < M; row++) {
		for (int col = 0; col < N; col++) {
			t[col * M + row] = data[row * N + col];
		}
	}
	for (int i = 0; i < M * N; i++) {
		data[i] = t[i];
	}
	delete[] t;
}

struct point {
	int x = 0;
	int y = 0;
	struct point* father = nullptr;
	double G = 0;
	double H = 0;
	double F = G + H;

};
bool comparator(const point* firstObj, const point* secondObj) {
	return firstObj->F > secondObj->F;
}
void sortOpenList(vector<point*>& list) {
	sort(list.begin(), list.end(), comparator);
}
double dis(int startX, int startY, int endX, int endY) {
	return sqrt((startX - endX) * (startX - endX) + (startY - endY) * (startY - endY));
}
double* AStar(int M, int N, double* graph, int startX, int startY, int endX, int endY) {

	colFirstToRowFirst(graph, M, N);
	if (graph[endX * N + endY] != 0||graph[startX*N+startY]!=0) {
		mexErrMsgTxt("Cannot solve the path without a right input,check your start or end Pos please.");
		return NULL;
	}

	// 0 未遍历，1 障碍物，2开启队列，-1关闭队列
	for (int i = 0; i < M * N; i++) {
		if (graph[i] != 0) {
			graph[i] = 1;
		}
	}
	vector<point*> openList;
	vector<point*> closeList;
	// 放入起始点
	point* startPoint = new point{ startX,startY,nullptr,0,dis(startX,startY,endX,endY) };
	openList.push_back(startPoint);
	graph[startX * N + startY] = 2;
	int px[] = { 0,1,1,1,0,-1,-1,-1 };
	int py[] = { -1,-1,0,1,1,1,0,-1 };
	//point temp{ startX,startY };

	while (!openList.empty()) {

		// search point with min F
		sortOpenList(openList);
		point* temp = openList.back();
		closeList.push_back(temp);
		openList.pop_back();
		graph[temp->x * N + temp->y] = -1;


		if (temp->x == endX && temp->y == endY) {

			double* res = new double[M * N]{};
			point* t = temp;
			while (t->father != nullptr) {
				res[t->x * N + t->y] = 127;
				t = t->father;
			}

			// delete掉动态分配的所有指针
			for (point*& var : closeList) {
				delete[] var;
			}
			for (point*& var : openList) {
				delete[]var;
			}

			rowFirstToColFirst(res, M, N);

			return res;

		}
		for (int i = 0; i < 8; i++) {
			int newX = temp->x + px[i];
			int newY = temp->y + py[i];




			if (newX < M && newY < N && newX >= 0 && newY >= 0) {// 位置合法
				if (graph[newX * N + newY] == 1) {// 排除障碍物
					continue;
				}

				// 排除不可达
				if (i == 1 && graph[(newX - 1) * N + newY] == 1 && graph[(newX * N + newY + 1)] == 1) {
					continue;
				}
				if (i == 3 && graph[(newX * N + newY - 1)] == 1 && graph[(newX - 1) * N + newY] == 1) {
					continue;
				}
				if (i == 5 && graph[(newX + 1) * N + newY] == 1 && graph[newX * N + newY - 1] == 1) {
					continue;
				}
				if (i == 7 && graph[newX * N + newY + 1] == 1 && graph[(newX + 1) * N + newY] == 1) {
					continue;
				}


				double plus = 0;
				switch (i) {
				case 1:
				case 3:
				case 5:
				case 7:
					plus = 1.414;
					break;
				default:
					plus = 1;
					break;
				}
				if (2 == graph[newX * N + newY]) {
					for (point*& var : openList) {
						// 找到这个在openList的点，如果从出发点到它有更短的路径，就更新
						if (var->x == newX && var->y == newY && var->G > temp->G + plus) {
							var->father = temp;
							var->G = temp->G + plus;	// upfate G
							var->F = var->G + var->H;	// update F
							break;
						}
					}

				}
				if (0 == graph[newX * N + newY]) {
					graph[newX * N + newY] = 2;
					point* newPoint = new point{ newX,newY,temp,temp->G + plus,dis(newX,newY,endX,endY) };
					openList.push_back(newPoint);
				}
			}



		}
	}


	mexErrMsgTxt("Cannot solve the path,for it doesn't exist.");

	return nullptr;
}

void mexFunction(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
	if (nrhs != 5) {
		mexErrMsgTxt("Check your input arguments,please.");
	}
	double* inData;
	double* outData;
	int mrows = mxGetM(prhs[0]);
	int nrows = mxGetN(prhs[0]);
	plhs[0] = mxCreateDoubleMatrix(mrows, nrows, mxREAL);
	inData = mxGetPr(prhs[0]);	
	outData = mxGetPr(plhs[0]); 

	if (!mxIsDouble(prhs[0]) || mxIsComplex(prhs[0])) {
		mexErrMsgTxt("Input must be a noncomplex scalar double.");
	}

	int startX = *(mxGetPr(prhs[1]));
	int startY = *(mxGetPr(prhs[2]));
	int endX = *(mxGetPr(prhs[3]));
	int endY = *(mxGetPr(prhs[4]));
	if (endX >= mrows || endX >= nrows || startX >= mrows || startY >= nrows) {
		mexErrMsgTxt("Please check your start and end Pos, which may out of index.");
	}
	if (endX < 0 || endY < 0 || startX < 0 || startY < 0) {
		mexErrMsgTxt("Illegal parameters!!");
	}
	double* copyOfInData = new double[mrows * nrows];// avoid modifying the original data;
	memcpy(copyOfInData, inData, mrows * nrows * sizeof(double));// cannot overlap;
	double* res = AStar(mrows, nrows, copyOfInData, startX, startY, endX, endY);
	memcpy(outData, res, mrows * nrows * sizeof(double));
	delete[] copyOfInData;
}