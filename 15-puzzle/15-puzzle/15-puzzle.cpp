﻿#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <vector>

#define SWAP(x, y) int tmp = x; x = y; y = tmp;
#define SIZE 4
#define FOUND -1

typedef struct Node {
	int board[16];	//tiles position
	int parent;
	int action; 
	int pathCost; 
	int depth;
}node;

enum Direction { UP, DOWN, LEFT, RIGHT };

std::vector<char> path;

int puzzle[16] = { 1,11,9,13,8,10,15,3,6,14,12,4,0,7,5,2 };
int puzzle_pos[16] = { 0 };

void GetPosition(int puzzle[16])
{
	for (int i = 0; i < 16; i++)
		puzzle_pos[puzzle[i]] = i;
}
/*
void RandomPuzzle()
{
	for (int i = 0; i < 16; i++)
	{
		int j = i + rand() % (16 - i);
		SWAP(puzzle[i], puzzle[j]);
	}
	for (int i = 0; i < 16; i++)
	{
		if ((i + 1) % 4 == 0)
			printf("%2d\n", puzzle[i]);
		else
			printf("%2d ", puzzle[i]);
	}
}*/

//compare current position with final position
//h(P) : the Manhattan distance between the current and the goal
int manhattanDistance(int *board)
{
	int dis = 0;
	for (int i = 0; i < 16; i++)
	{
		if (puzzle[i] == 0)
			continue;
		dis += abs((board[i] / SIZE) - (i % SIZE)) + abs((board[i] % SIZE) - (i % SIZE));
	}
	return dis;
}

//g(P) : the number of moves made so far.
bool idaStar(node *node, int g)
{
	int threshold = manhattanDistance(node->board);
	while (true)
	{
		int result = dfs(node, 0, threshold);
		if (result == FOUND) return true;
		threshold = result;
	}
	return false;
}

int dfs(Node *node, int g, int threshold)
{
	//Evaluation value h, f() = g + h
	int h = manhattanDistance(node->board);
	//Find the final answer
	if (h == 0) return FOUND;
	//Pruning
	int f = g + h;
	if (f > threshold) return f; //update threshold

}

int main()
{
	srand(time(NULL));
	//RandomPuzzle();
	char finalAnswer[16] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,0 };
}