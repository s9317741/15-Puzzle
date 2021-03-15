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
}node;

enum Direction { UP, DOWN, LEFT, RIGHT };

int dir[4] = { -4, +4, -1, +1 };

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

void printPath(Node* root)
{
	if (root == NULL)
		return;
	printPath(root->parent);
	printBoard(root->board);

	printf("\n");
}
//UP:0, DOWN:1, left:2, right:3
bool isLegal(int pos, int dir)
{
	if ((pos + dir) < 0 || (pos + dir) > 15) return false;
	if ((pos % 4 == 0 && dir == 2) || (pos / 4 == 0 && dir == 0)
		|| (pos % 4 == 3 && dir == 3) || (pos / 4 == 3 && dir == 1))
		return false;
	return true;
}

bool dfs(Node *node, int g, int threshold)
{
	//Evaluation value h, f() = g + h
	int h = manhattanDistance(node->board);
	//Find the final answer
	if (h == 0) return true;
	//Pruning
	int f = g + h;
	if (f > threshold) return false; //update threshold
	for (int i = 0; i < 4; i++)
	{
		if ((node->action ^ 1) != i && isLegal(node->pos, dir[i]))
		{
			path.push_back(i);
			Node *child = newNode(node->board, node, node->pos, node->pos + dir[i], i, node->depth + 1);
			if (dfs(child, g + 1, threshold)) return true;
			path.push_back(i);
			delete(child);
		}
	}
	return false;
}
/*
void dfs(int board[16])
{
	//Evaluation value h, f() = g + h
	int h = manhattanDistance(node->board);
	//Find the final answer
	if (h == 0) return FOUND;
	//Pruning
	int f = g + h;
	if (f > threshold) return f; //update threshold
	int pos;
	for (int i = 0; i < 16; i++)
	{
		if (board[i] == 0)
		{
			pos = i;
			break;
		}
	}
		

	std::stack<Node*> _stack;
	Node* root = newNode(board, NULL, pos, pos, NULL, 0);
	_stack.push(root);
	while (!_stack.empty())
	{
		Node *cur = _stack.top();
		_stack.pop();

		if (cur->pathCost == 0)
		{
			printPath(cur);
			//return FOUND;
			return;
		}

		for (int i = 0; i < 4; i++)
		{
			if (cur->action != i && isLegal(cur->pos, dir[i]))
			{
				Node *child = newNode(cur->board, cur, cur->pos, cur->pos + dir[i], i, cur->depth+1);

				child->pathCost = manhattanDistance(child->board);
				_stack.push(child);
			}
		}
	}
	//return 0;
}*/


//g(P) : the number of moves made so far.
bool idaStar()
{
	int pos;
	for (int i = 0; i < 16; i++)
	{
		if (puzzle[i] == 0)
		{
			pos = i;
			break;
		}
	}
	Node *root = newNode(puzzle, NULL, pos, pos, NULL, 0);;
	int threshold = manhattanDistance(root->board);
	while (true)
	{
		//int result = dfs(node, 0, threshold);
		//if (result == FOUND) return true;
		if (dfs(root, 0, threshold)) return true;
		//threshold = result;
	}
	return false;
}

int main()
{
	srand(time(NULL));
	RandomPuzzle();
	//printBoard(puzzle);	
	idaStar();
	//dfs(puzzle);
}