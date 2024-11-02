#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 20
#define WALL '#'
#define PATH ' '
#define VISITED '.'

// 迷宫和其尺寸
char maze[MAX_SIZE][MAX_SIZE];
int width, height;

// 读取迷宫文件
int readMaze(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("无法打开文件");
        return 0;
    }

    // 读取宽度和高度
    fscanf(file, "%d,%d", &width, &height);

    // 读取迷宫布局
    for (int i = 0; i < height; i++) {
        fscanf(file, "%s", maze[i]);
    }

    fclose(file);
    return 1;
}

// 打印迷宫
void printMaze() {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            printf("%c", maze[i][j]);
        }
        printf("\n");
    }
}

// 深度优先搜索函数，用递归找到路径
int searchPath(int x, int y) {
    // 检查是否在迷宫边界内
    if (x < 0 || x >= height || y < 0 || y >= width) return 0;

    // 检查是否到达出口
    if (x == height - 1 || y == width - 1 || x == 0 || y == 0) {
        if (maze[x][y] == PATH) {
            maze[x][y] = VISITED; // 标记出口位置
            return 1; // 找到出口
        }
    }

    // 检查当前位置是否可通行
    if (maze[x][y] != PATH) return 0;

    // 标记当前路径为已访问
    maze[x][y] = VISITED;

    // 递归搜索四个方向
    if (searchPath(x + 1, y) || // 向下
        searchPath(x - 1, y) || // 向上
        searchPath(x, y + 1) || // 向右
        searchPath(x, y - 1)) { // 向左
        return 1;
    }

    // 回溯，将当前位置重置为可通行路径
    maze[x][y] = PATH;
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("请提供迷宫文件名。\n");
        return 1;
    }

    // 读取迷宫
    if (!readMaze(argv[1])) {
        printf("无法读取迷宫文件。\n");
        return 1;
    }

    // 从入口开始搜索路径（假设入口在左上角附近）
    if (searchPath(0, 0)) {
        printf("找到路径:\n");
        printMaze();
        return 0; // 成功找到路径，返回状态 0
    } else {
        printf("没有找到可行路径。\n");
        return 1; // 未找到路径，返回状态 1
    }
}
