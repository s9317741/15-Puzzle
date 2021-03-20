#include <stdio.h>
#include <time.h>
#include <vector>
#include <stack>
#include <unordered_map>
#include <string>

#define SWAP(x, y) int tmp = x; x = y; y = tmp;
#define SIZE 4
#define FOUND -1

typedef struct Node {
	int board[16];	// tiles position
	Node *parent;
	int pos;
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

int puzzle[16] = { 0 };
int finalAnswer[16] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,0 };

// Function to allocate a new node
Node* newNode(int *board, Node *parent, int pos, int newPos, int depth)
{
	Node* node = new Node();

	node->parent = parent;

	// copy data from parent node to current node
	memcpy(node->board, board, sizeof(node->board));

	// move tile by 1 position
	SWAP(node->board[pos], node->board[newPos]);

	// set number of moves so far
	node->depth = depth;

	node->pos = newPos;

	return node;
}

void RandomPuzzle()
{
	int num = 3;// rand() % 5;
	int temp0[16] = { 0 ,4, 2, 3, 13, 8, 7, 6, 5, 10, 11, 1, 9, 12, 15, 14 }; //best 52
	int temp1[16] = { 5, 1, 0, 4, 7, 6, 2, 3, 9, 10, 12, 8, 13, 14, 11, 15 }; //16
	int temp2[16] = { 6, 1, 7, 4, 5, 3, 11, 8, 2, 14, 12, 15, 9, 10, 13, 0 }; //22
	int temp3[16] = { 5, 2, 7, 4, 3, 1, 12, 0, 10, 13, 9, 6, 14, 8, 11, 15 }; //best 34
	int temp4[16] = { 8, 0, 2, 4, 5, 14, 3, 1, 9, 10, 7, 12, 13, 11, 15, 6 }; //best 37
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

void GenPuzzle()
{
	for (int i = 0; i < 10; i++)
	{
		int a = rand() % 16, b = rand() % 16;
		SWAP(puzzle[a], puzzle[b]);
	}
	for (int i = 0; i < 16; i++)
		printf("%d, ", puzzle[i]);
}

void getBoardPosition(int *board)
{
	int *boardPos = new int[16];
	for (int i = 0; i < 16; i++)
		boardPos[board[i]] = i;
	for (int i = 0; i < 16; i++)
		printf("%d, ", boardPos[i]);

	//return boardPos;
}

// compare current position with final position
// h(P) : the Manhattan distance between the current and the goal
int manhattanDistance(int *board)
{
	int dis = 0;
	//int *getPos = getBoardPosition(board);
	//int *getPos1 = getBoardPosition(finalAnswer);
	for (int i = 0; i < 16; i++)
	{
		if (board[i] == 0)
			continue;
		dis += abs(board[i] / SIZE - finalAnswer[i] / SIZE ) + abs(board[i] % SIZE - finalAnswer[i] % SIZE);
	}
	return dis;
}

void printBoard(int *board)
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
		a += board[i]+'0';
	}
	return a;
}

// recussive
int dfs_r(std::stack<Node*> stack, int g, int threshold)
{
	Node *node = stack.top();
	// Evaluation value h, f() = g + h
	std::string k = intBoardToArray(node->board);
	h_map.insert(std::pair<std::string, bool>(k, true));
	int h = manhattanDistance(node->board);
	// Pruning
	int f = g + h;
	if (f > threshold) return f; // update threshold
		// Find the final answer
	if (h == 0)
	{
		printPath(node);
		return FOUND;
	}
	int min = INT_MAX;

	for (int i = 0; i < 4; i++)
	{
		if (isLegal(node->pos, i))
		{
			Node *child = newNode(node->board, node, node->pos, (node->pos + dir[i]), (node->depth + 1));
			std::string key = intBoardToArray(child->board);			
			if (h_map.find(key) != h_map.end()) {
				//printf("HI\n");
				delete(child);
				continue;
			}
			path.push_back(i);		
			stack.push(child);			
			int result = dfs_r(stack, g + 1, threshold) ;			
			if (result == FOUND) return FOUND;
			if (result < min) min = result;	
			path.pop_back();
			stack.pop();
			delete(child);
		}	
	}
	return min;
}

// g(P) : the number of moves made so far.
bool idaStar(int pos)
{
	Node *root = newNode(puzzle, NULL, pos, pos, 0);
	int threshold = manhattanDistance(root->board);
	std::stack<Node*> stack;
	stack.push(root);
	while (true)
	{
		h_map.clear();
		//std::string key = intBoardToArray(root->board);
		//h_map.insert(std::pair<std::string, bool>(key, true));
		int result = dfs_r(stack, 0, threshold);
		if (result == FOUND) return true;
		if (result == INT_MAX) {
			printf("result == INT_MAX\n");
			return false;
		}
		threshold = result;
	}
	return false;
}

bool isSolvable(int *board, int pos) {
	
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
	getBoardPosition(puzzle);
	//GenPuzzle();
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