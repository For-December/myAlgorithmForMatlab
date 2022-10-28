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

struct pos {
	int x;
	int y;
};

double* BFS(int M, int N, double* graph, int startX, int startY, int endX, int endY) {
	queue<pos> q;
	int* path = new int[M * N]{};
	int px[] = { -1,0,1,0 };
	int py[] = { 0,-1,0,1 };
	//int px[] = { -1,-1,0,1,1,1,0,-1 }; // all directions!
	//int py[] = { 0,-1,-1,-1,0,1,1,1 };
	colFirstToRowFirst(graph, M, N);
	pos start = { startX,startY };
	if (graph[endX * N + endY] != 0||graph[startX*N+startY]!=0) {
		mexErrMsgTxt("Cannot solve the path without a right input,check your start or end Pos please.");
		return NULL;
	}
	graph[startX*N+startY] = -1;
	q.push(start);
	while (!q.empty()) {
		pos temp = q.front();
		q.pop();
		for (int i = 0; i < 4; i++) {// 一个点有四=>八个可能的方向
			int newX = temp.x + px[i];
			int newY = temp.y + py[i];

			if (newX == endX && newY == endY) {
				graph[newX * N + newY] = -1;
				path[newX * N + newY] = temp.x * N + temp.y;
				stack<pos> s;
				int front = endX * N + endY;
				s.push(pos{ endX,endY });
				while (front != startX*N+startY) {
					front = path[front];
					s.push(pos{ int(front / N),(front - int(front / N) * N) });
				}
				double* res = new double[M * N]{};
				while (!s.empty()) {
					pos t = s.top();
					s.pop();
					res[t.x * N + t.y] = 127;
				}
				rowFirstToColFirst(res, M, N);
				return res;
			}
			if (newX < M && newY < N && newX >= 0 && newY >= 0 && graph[newX * N + newY] == 0) {
				graph[newX * N + newY] = -1;
				path[newX * N + newY] = temp.x * N + temp.y;
				q.push(pos{ newX,newY });

			}
		}
	}
	mexErrMsgTxt("Cannot solve the path,for it doesn't exist.");
	return NULL;

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
	double* res = BFS(mrows, nrows, copyOfInData, startX, startY, endX, endY);
	memcpy(outData, res, mrows * nrows * sizeof(double));
	delete[] copyOfInData;
}