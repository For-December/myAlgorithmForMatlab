# myAlgorithmOnMatlab

~~Labyrinth walking based on BFS (which may be changed to A * later)? Seeking the way? Algorithm~~

- A * routing algorithm based on BFS and Dijkstra algorithm has been implemented

- Republished

# This is only a test project for my homework, and the implementation method is finally called by matlab



- The core files are+myBFS. cpp+and+myAStar. cpp



## How to use?

Preparing environment in matlab



```matlab



%Building environment

mex -setup

% Select the C++environment when prompted



% Building environment

mbuild -setup

% If MATLAB Compiler SDK is not installed, the command line will respond to the prompt



```

- After that, you can use the mex command to compile the. cpp file of the required algorithm and use the



- You can also directly place the required. mexw64 file in the project to the same directory as the. m file



# About usage



```matlab

% Take myAStar as an example. The other parameter is consistent

MyAStar (matrix, starting point x, starting point y, ending point x, ending point y);



```

The return value is the shortest path matrix of the same type as the "matrix", which is only composed of 0 and 127 (the shortest in pixels, not necessarily visually)


# myAlgorithmOnMatlab
~~基于BFS(之后或许会改为A*)的走迷宫？寻路？算法~~

- 基于BFS和迪杰斯特拉算法的A*寻路算法已实现

- 已重新发布

# 只是个为作业服务的测试项目，实现的方法最终交由matlab调用

- 核心文件是+myBFS.cpp+以及+myAStar.cpp

## 如何调用？
在matlab中准备环境

```matlab

% 搭建环境
mex -setup
% 提示出现后选择C++环境

% 搭建环境
mbuild -setup
% 如未安装MATLAB Compiler SDK 命令行会响应提示

```
- 之后可自行使用mex命令编译所需算法的.cpp文件并使用

- 也可直接将项目中的所需.mexw64文件放至.m文件的同级目录，直接使用

# 关于使用方法

```matlab
% 以myAStar为例，另一个参数一致
myAStar(矩阵,起始点x,起始点y,终止点x,终止点y);

```
返回值为和"矩阵"同型的，仅由0和127组成的最短路径矩阵(像素上最短，视觉上不一定)
