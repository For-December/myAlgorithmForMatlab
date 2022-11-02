#include <iostream>
#include <queue>
#include <stack>
#include <string>
#include <fstream>
#include <math.h>
using namespace std;
void colFirstToRowFirst(double* data, int M, int N) {
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

// 内部有输出
double* BFS2(int M, int N, int* graph, int startX, int startY, int endX, int endY) {
	queue<pos> q;
	int* path = new int[M * N]{};
	int px[] = { -1,0,1,0 };
	int py[] = { 0,-1,0,1 };
	pos start = { startX,startY };
	if (graph[endX * N + endY] != 0 || graph[startX * N + startY] != 0) {
		return NULL;
	}
	graph[startX*N+startY] = -1;
	q.push(start);
	while (!q.empty()) {
		pos temp = q.front();
		q.pop();
		for (int i = 0; i < 4; i++) {// 一个点有四个可能的方向
			int newX = temp.x + px[i];
			int newY = temp.y + py[i];

			if (newX == endX && newY == endY) {
				graph[newX * N + newY] = -1;
				path[newX * N + newY] = temp.x * N + temp.y;
				stack<pos> s;
				int front = endX * N + endY;
				s.push(pos{endX,endY});
				while (front!=startX*N+startY) {
					front = path[front];
					s.push(pos{ front / N,(front-int(front/N)*N)});
				}
				double* res = new double[M * N]{};
				while (!s.empty()) {
					pos t = s.top();
					s.pop();
					res[t.x*N+t.y] = 3;
				}
				for (int j = 0; j < M; j++) {
					for (int k = 0; k < N; k++) {
						printf("%.0f\t", res[j * N + k]);
					}
					printf("\n");
				}
				return res;
			}
			if (newX<M && newY<N && newX>=0 && newY>=0 && graph[newX*N+newY] == 0) {
				graph[newX*N+newY] = -1;
				path[newX * N + newY] = temp.x * N + temp.y;
				q.push(pos{ newX,newY });

			}
		}
	}
	return NULL;

}
// 内部无输出
double* BFS(int M, int N, double* graph, int startX, int startY, int endX, int endY) {
	queue<pos> q;
	int* path = new int[M * N]{};
	int px[] = { -1,-1,0,1,1,1,0,-1 };
	int py[] = { 0,-1,-1,-1,0,1,1,1 };
	//int px[] = { -1,0,1,0 };
	//int py[] = { 0,-1,0,1 };
	colFirstToRowFirst(graph, M, N);
	pos start = { startX,startY };
	if (graph[endX * N + endY] != 0 || graph[startX * N + startY] != 0) {
		return NULL;
	}
	graph[startX * N + startY] = -1;
	q.push(start);
	while (!q.empty()) {
		pos temp = q.front();
		q.pop();
		for (int i = 0; i < 8; i++) {// 一个点有四=>八个可能的方向

			int newX = temp.x + px[i];
			int newY = temp.y + py[i];

			if (newX == endX && newY == endY) {
				graph[newX * N + newY] = -1;
				path[newX * N + newY] = temp.x * N + temp.y;
				stack<pos> s;
				int front = endX * N + endY;
				s.push(pos{ endX,endY });
				while (front != startX * N + startY) {
					front = path[front];
					s.push(pos{ int(front / N),(front - int(front / N) * N) });
				}
				double* res = new double[M * N]{};
				while (!s.empty()) {
					pos t = s.top();
					s.pop();
					res[t.x * N + t.y] = 127;
				}
				return res;
				rowFirstToColFirst(res, M, N);
			}
			if (newX < M && newY < N && newX >= 0 && newY >= 0 && graph[newX * N + newY] == 0) {
				graph[newX * N + newY] = -1;
				path[newX * N + newY] = temp.x * N + temp.y;
				q.push(pos{ newX,newY });

			}
		}
	}
	return NULL;

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
			res[t->x * N + t->y] = 127;
			while (t->father != nullptr) {
				t = t->father;
				res[t->x * N + t->y] = 127;
			}

			// delete掉动态分配的所有指针
			for (point*& var : closeList) {
				delete[] var;
			}
			for (point*& var : openList) {
				delete[]var;
			}

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



	return nullptr;
}
double solveCostOfCurPath(int M, int N, double* path, int startX, int startY, int endX, int endY) {
	// 记录走过的路
	double* temp = new double[M * N]{};

	int px[] = { 0,1,1,1,0,-1,-1,-1 };
	int py[] = { -1,-1,0,1,1,1,0,-1 };
	int curX = startX;
	int curY = startY;
	double count = 0;
	temp[startX * N + startY] = -1; // 初始化初始点，否则死循环
	int err = 0;
	while (true) {
		err++;
		if (err > M * N) {
			delete[] temp;
			return -1;
		} // 循环超过M*N次，说明输入数据不合适
		if (curX == endX && curY == endY) {
			delete[] temp;
			return count;
		}
		for (int i = 0; i < 8; i++) {
			if (curX + px[i] >= M || curY + py[i] >= N || curX + px[i] < 0 || curY + py[i] < 0)continue;

			if (-1 == temp[(curX + px[i]) * N + (curY + py[i])])continue;
			if (abs(127 - path[(curX + px[i]) * N + (curY + py[i])]) < 0.1) {
				switch (i) {
				case 1:
				case 3:
				case 5:
				case 7:
					count += 1.414;
					break;
				default:
					count += 1;
					break;
				}
				curX += px[i];
				curY += py[i];
				temp[curX * N + curY] = -1;
				break;
			}
		}
	}

}


#define MYSIZE 6
int main() {


	int n[5][5] = { 0 }; // 只要赋值，未被赋值的元素默认初始化为0，不{}赋值结果未定义
	int** m = new int* [6];
	for (int i = 0; i < 6; i++) {
		// 方法一
//		m[i] = new int[5]{};
		// 方法二
		m[i] = new int[6];
		memset(m[i], 0, 6 * sizeof(int));
	}
	//double t[MYSIZE][MYSIZE] = {};
	double t[6][6] = {};
	//{
	//	0,0,0,0,0,0,
	//	1,1,1,0,0,1,
	//	1,0,0,1,0,1,
	//	1,0,1,0,0,0,
	//	1,0,0,0,1,0,
	//	1,1,1,1,1,0
	//};
	
	
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 6; j++) {
//			printf("%d", m[i][j]);
			m[i][j] = t[i][j];
		}
	}
	
	double*path=AStar(MYSIZE, MYSIZE, (double*)t, 0, 0, 1, 2);

	for (int i = 0; i < MYSIZE; i++) {
		for (int j = 0; j < MYSIZE; j++) {
			cout << path[i * MYSIZE + j] << "\t";
		}
		cout << endl;
	}

//	path[0, 0] = 0;
	//cout << solveCostOfCurPath(MYSIZE, MYSIZE, path, MYSIZE-1, MYSIZE-1, 0, 0);
	//cout << (solveCostOfCurPath(MYSIZE, MYSIZE, path, MYSIZE - 1, MYSIZE - 1, 0, 0) == -1);



	cout << solveCostOfCurPath(MYSIZE, MYSIZE, path,0,0,1,2);




//	for (int i = 0; i < 5; i++) {
//		for (int j = 0; j < 5; j++) {
//			printf("%d\t", m[i][j]);
//		}
//		printf("\n");
//	}

	return 0;
}
