#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <vector>
#include <stack>

#define SWAP(x, y) int tmp = x; x = y; y = tmp;
#define SIZE 4
#define FOUND -1

typedef struct Node {
	int board[16];	//tiles position
	Node *parent;
	int pos;
	int action; 
	int pathCost; 
	int depth;
};

//UP, DOWN, LEFT, RIGHT
int dir[4] = { -4, +4, -1, +1 };

char dir_s[4][10] = { "Up", "Down", "Left", "Right" };

std::vector<char> path;

int puzzle[16] = {0};
const int finalAnswer[16] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,0 };

// Function to allocate a new node
Node* newNode(int board[16], Node* parent, int pos, int newPos, int action, int depth)
{
	Node* node = new Node;

	// set pointer for path to root
	node->parent = parent;

	// copy data from parent node to current node
	memcpy(node->board, board, sizeof node->board);

	// move tile by 1 position
	SWAP(node->board[pos], node->board[newPos]);

	// set number of misplaced tiles
	node->pathCost = INT_MAX;

	// set number of moves so far
	node->depth = depth;

	node->pos = newPos;

	node->action = action;

	return node;
}

void RandomPuzzle()
{
	int num = rand() % 5;
	int temp0[16] = { 0,4,2,3,13,8,7,6,5,10,11,1,9,12,15,14 };
	int temp1[16] = { 12,8,3,13,10,6,15,2,4,5,1,9,11,14,7,0 };
	int temp2[16] = { 1,5,9,13,2,6,10,14,3,7,11,15,4,8,12,0 }; //difficult
	int temp3[16] = { 0,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1 };
	int temp4[16] = { 0,12,9,13,15,11,10,14,7,8,5,6,4,3,2,1 };
	switch (num)
	{
	case 0:
		memcpy(puzzle, temp0, sizeof(puzzle));
		break;
	case 1:
		memcpy(puzzle, temp1, sizeof(puzzle));
		break;
	case 2:
		memcpy(puzzle, temp2, sizeof(puzzle));
		break;
	case 3:
		memcpy(puzzle, temp3, sizeof(puzzle));
		break;
	case 4:
		memcpy(puzzle, temp4, sizeof(puzzle));
		break;
	default:
		printf("Error Num in Random\n");
		break;
	}
	for (int i = 0; i < 16; i++)
	{
		if ((i + 1) % 4 == 0)
			printf("%2d\n", puzzle[i]);
		else
			printf("%2d ", puzzle[i]);
	}
}

//compare current position with final position
//h(P) : the Manhattan distance between the current and the goal
int manhattanDistance(int *board)
{
	int dis = 0;
	for (int i = 0; i < 16; i++)
	{
		if (board[i] == 0)
			continue;
		dis += abs((board[i] / SIZE) - (finalAnswer[i] / SIZE)) + abs((board[i] % SIZE) - (finalAnswer[i] % SIZE));
	}
	return dis;
}

void printBoard(int board[16])
{
	for (int i = 0; i < 16; i++)
	{
		printf("%2d ", board[i]);
		if(i % 4 == 3) printf("\n");
	}
}

void printPath(Node* node)
{
	if (node == NULL)
		return;
	printPath(node->parent);
	printBoard(node->board);

	printf("\n");
}

int FindBlankPos()
{
	for (int i = 0; i < 16; i++)
		if (puzzle[i] == 0)
			return i;
}

//UP:0, DOWN:1, left:2, right:3
bool isLegal(int pos, int direction)
{
	if ((pos + dir[direction]) < 0 || (pos + dir[direction]) > 15) return false;
	if ((pos % 4 == 0 && direction == 2) || (pos / 4 == 0 && direction == 0)
		|| (pos % 4 == 3 && direction == 3) || (pos / 4 == 3 && direction == 1))
		return false;
	return true;
}

bool dfs(Node *node, int g, int threshold)
{
	//Evaluation value h, f() = g + h
	int h = manhattanDistance(node->board);
	//Find the final answer
	if (h == 0) 
	{
		printPath(node);
		return true;
	}
	//Pruning
	int f = g + h;
	if (f > threshold) return false; //update threshold
	Node *child = new Node();
	for (int i = 0; i < 4; i++)
	{
		if (node->action == NULL || (dir[node->action] != (-dir[i]) && isLegal(node->pos, i)))
		{
			path.push_back(i);
			child = newNode(node->board, node, node->pos, node->pos + dir[i], i, node->depth + 1);
			if (dfs(child, g + 1, threshold)) return true;
			path.pop_back();
		}
	}
	delete(child);
	return false;
}

//g(P) : the number of moves made so far.
bool idaStar(int pos)
{
	Node *root = newNode(puzzle, NULL, pos, pos, NULL, 0);
	int threshold = manhattanDistance(root->board);
	clock_t start = clock();
	while (true)
	{
		if (dfs(root, 0, threshold)) return true;
	}
	clock_t end = clock();
	return false;
}

int main()
{
	srand(time(NULL));
	//RandomPuzzle();	
	//{ 5,1,0,4,7,6,2,3,9,10,12,8,13,14,11,15 }	
	//{ 6, 1, 7, 4, 5, 3, 11, 8, 2, 14, 12, 15, 9, 10, 13, 0 }
	int temp[16] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };
	memcpy(puzzle, temp, sizeof(temp));
	printBoard(puzzle);
	printf("\n");
	int pos = FindBlankPos();
	
	if (idaStar(pos))
	{
		int count = 1;
		for (int i = 0; i < path.size(); i++, count++)
			printf("%d. %s\n", count, dir_s[path.at(i)]);
	}

}