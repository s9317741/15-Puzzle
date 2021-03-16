#include <stdio.h>
#include <time.h>
#include <vector>
#include <stack>
#include <unordered_map>
#include <string>
#include <sstream>

#define SWAP(x, y) int tmp = x; x = y; y = tmp;
#define SIZE 4
#define FOUND -1

typedef struct Node {
	int board[16];	// tiles position
	Node *parent;
	int pos;
	int action; 
	int depth;
};

enum Direction
{
	UP, DOWN, LEFT, RIGHT
};

// UP, DOWN, LEFT, RIGHT
int dir[4] = { -4, +4, -1, +1 };

char dir_s[4][10] = { "Up", "Down", "Left", "Right" };

std::vector<char> path;
std::unordered_map<std::string, bool> h_map;

int puzzle[16] = {0};
const int finalAnswer[16] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,0 };

// Function to allocate a new node
Node* newNode(int *board, Node *parent, int pos, int newPos, int action, int depth)
{
	Node* node = new Node;

	node->parent = parent;

	// copy data from parent node to current node
	memcpy(node->board, board, sizeof(node->board));

	// move tile by 1 position
	SWAP(node->board[pos], node->board[newPos]);

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
	int temp2[16] = { 1,5,9,13,2,6,10,14,3,7,11,15,4,8,12,0 }; // difficult
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

// compare current position with final position
// h(P) : the Manhattan distance between the current and the goal
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

int FindBlankPos(int *board)
{
	for (int i = 0; i < 16; i++)
		if (board[i] == 0)
			return i;
}

// UP:0, DOWN:1, left:2, right:3
bool isLegal(int pos, int direction)
{
	if ((pos + dir[direction]) < 0 || (pos + dir[direction]) > 15) return false;
	if ((pos % 4 == 0 && direction == LEFT) || (pos / 4 == 0 && direction == UP)
		|| (pos % 4 == 3 && direction == RIGHT) || (pos / 4 == 3 && direction == DOWN))
		return false;
	return true;
}

std::string intBoardToArray(int *board)
{
	std::string a;
	for (int i = 0; i < 16; i++)
	{
		a += board[i]+'a';
	}
	return a;
}

// recussive
int dfs_r(std::stack<Node*> &stack, int g, int threshold)
{
	Node *node = stack.top();

	// Evaluation value h, f() = g + h
	int h = manhattanDistance(node->board);
	// Pruning
	int f = g + h;
	if (f > threshold) return f; // update threshold
	// Find the final answer
	if (h == 0) 
	{
		//printPath(node);
		return FOUND;
	}
	int min = INT_MAX;

	for (int i = 0; i < 4; i++)
	{
		if ((node->action == NULL || dir[node->action]+dir[i] != 0 ) && isLegal(node->pos, i))
		{
			Node *child = newNode(node->board, node, node->pos, (node->pos + dir[i]), i, (node->depth + 1));
			std::string key = intBoardToArray(child->board);
			//printf("%s\n", key);
			/*
			if (h_map.find(key) != h_map.end()) {
				//printf("HI\n");
				continue;
			}
			h_map.insert(std::pair<std::string, bool>(key, true));*/
			path.push_back(i);		
			stack.push(child);			
			int result = dfs_r(stack, g + 1, threshold) ;
			if (result == FOUND) return FOUND;
			if (result < min) min = result;
			path.pop_back();
			stack.pop();			
		}
	}
	return min;
}

// g(P) : the number of moves made so far.
bool idaStar(int pos)
{
	Node *root = newNode(puzzle, NULL, pos, pos, NULL, 0);
	int threshold = manhattanDistance(root->board);
	std::stack<Node*> stack;
	stack.push(root);
	while (true)
	{
		int result = dfs_r(stack, 0, threshold);
		if (result == FOUND) return true;
		if (result == INT_MAX) return false;
		threshold = result;
	}
	return false;
}

bool isSolvable(int board[16], int pos) {
	
	int count = 0;
	for (int i = 0; i < 15; i++) {
		for (int j = i + 1; j < 16; j++)
			if (board[i] > board[j] && board[i] != 0 && board[j] != 0) {
				count++;
			}
	}
	return (count % 2 + (pos / SIZE)) % 2;
}

int main()
{
	srand(time(NULL));
	RandomPuzzle();	
	// 5, 1, 0, 4, 7, 6, 2, 3, 9, 10, 12, 8, 13, 14, 11, 15	
	// 6, 1, 7, 4, 5, 3, 11, 8, 2, 14, 12, 15, 9, 10, 13, 0
	// 0, 4, 2, 3, 13, 8, 7, 6, 5, 10, 11, 1, 9, 12, 15, 14
	/*int n;
	scanf("%d", &n);
	int temp1[16] = { 5, 1, 0, 4, 7, 6, 2, 3, 9, 10, 12, 8, 13, 14, 11, 15 };
	int temp2[16] = { 6, 1, 7, 4, 5, 3, 11, 8, 2, 14, 12, 15, 9, 10, 13, 0 };
	switch (n)
	{
	case 1:
		memcpy(puzzle, temp1, sizeof(temp1));
		break;
	case 2:
		memcpy(puzzle, temp2, sizeof(temp2));
		break;
	}*/
	printBoard(puzzle);
	printf("\n");

	int pos = FindBlankPos(puzzle);

	if (isSolvable(puzzle, pos))
	{	
		clock_t start = clock();
		bool result = idaStar(pos);
		clock_t end = clock();

		if (result)
		{
			int count = 1;
			for (int i = 0; i < path.size(); i++, count++)
				printf("%d. %s\n", count, dir_s[path.at(i)]);
		}
	}
	else
		printf("No solution\n");
}