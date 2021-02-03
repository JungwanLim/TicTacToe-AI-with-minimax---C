#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BOARDSIZE 3
#define true  1
#define false 0
#define Computer 1
#define User     2
#define max(a, b) (((a) > (b)) ? (a) : (b))
#define min(a, b) (((a) < (b)) ? (a) : (b))

int CheckedCount = 0; //�̹� �� �ڸ��� �� 9�� �Ǹ� ��� �� 
int winner = 0; //Computer = 1, User = 2
int maxDepth = 9; //�� �� �ձ��� �ξ���� ���� 
int bestPosition[2] = {-1, -1}; //����ü�� �ұ� �ϴ� �����ϰ� �迭�� ��� Mimimax �Լ����� ��� 

//1���� ����(1-9)�� �Էµ� ���� ��ǥ ������ ��ȯ�ϱ� ���� ����ü 
typedef struct tagPoint{
	int row;
	int col;
}Point;

// 1~9������ ���ڸ� 3X3 2���� �迭���� ��ǥ�� ��ȯ 
Point GetPoint(int position)
{
	Point p;
	p.row = position / BOARDSIZE;
	p.col = position % BOARDSIZE;
	return p;
}

// �Է��� 1�����̰�, ����� 2�������� �Ǿ� �־� �Էµ� ��ġ�� üũ�Ǿ� �ִ��� �˻� 
int isFill(int (*board)[BOARDSIZE], int position)
{
	Point p = GetPoint(position);
	if(board[p.row][p.col])
	{
		return true;
	}
	return false;
}

// ���忡 üũ�� ��ġ�� ���ڷ� �Է¹ޱ����� �Լ� 
int GetNumber(int (*board)[BOARDSIZE])
{
	char number; 
	while(true)
	{
		printf("Which position do you want? Input number(1 ~ 9) : ");
		scanf(" %c", &number);
		if(number < '1' || number > '9')
		{
			printf("\nPlease enter the correct number.\n");
		}
		else if(isFill(board, number - '1'))
		{
			printf("Fill position, Please enter the other number\n");
		}
		else
		{
			break;
		}
	}
	return number - '1';
}

// ���带 0���� �ʱ�ȭ 
void InitBoard(int (*board)[BOARDSIZE])
{
	int i, j;
	for(i = 0; i < BOARDSIZE; i++)
	{
		for(j = 0; j < BOARDSIZE; j++)
		{
			board[i][j] = 0;
		}
	}
}

// ������ ���¸� ȭ�鿡 ǥ�� 
void PrintBoard(int (*board)[BOARDSIZE])
{
	int i, j;
	char symbol[3] = {' ', 'X', 'O'};
	for(i = 0; i < BOARDSIZE; i++)
	{
		for(j = 0; j < BOARDSIZE; j++)
		{
			if(board[i][j])
			{
				printf("%c ", symbol[board[i][j]]);
			}
			else
			{
				printf("%d ", i * BOARDSIZE + j + 1);
			}
		}
		printf("\n");
	}
	printf("\n");
}

// User �Ǵ� Computer�κ��� �Էµ� ���� ���忡 ���� 
void SetNumber(int (*board)[BOARDSIZE], int position, int player)
{
	Point p = GetPoint(position);
	board[p.row][p.col] = player;
	CheckedCount++;
}

// �־��� ��ġ�� 0���� ����  
void SetZero(int (*board)[BOARDSIZE], int position)
{
	Point p = GetPoint(position);
	board[p.row][p.col] = 0;
	CheckedCount--;
}

// �¸�üũ 
int isWin(int (*board)[BOARDSIZE], int player)
{
	if( (board[0][0] == player && board[0][0] == board[0][1] && board[0][1] == board[0][2]) ||
		(board[1][0] == player && board[1][0] == board[1][1] && board[1][1] == board[1][2]) ||
		(board[2][0] == player && board[2][0] == board[2][1] && board[2][1] == board[2][2]) ||
		(board[0][0] == player && board[0][0] == board[1][0] && board[1][0] == board[2][0]) ||
		(board[0][1] == player && board[0][1] == board[1][1] && board[1][1] == board[2][1]) ||
		(board[0][2] == player && board[0][2] == board[1][2] && board[1][2] == board[2][2]) ||
		(board[0][0] == player && board[0][0] == board[1][1] && board[1][1] == board[2][2]) ||
		(board[2][0] == player && board[2][0] == board[1][1] && board[1][1] == board[0][2]) )
	{
		winner = player;
		return true;	
	} 
	
	return false;
}

// ������ ���� ���� ���� ����  
int evaluation()
{
	int score = 0;
	if(winner == Computer)
	{
		score = 1;
	}
	else if(winner == User)
	{
		score = -1;
	}
	winner = 0;
	return score;
}

// ������ �¸��� �߰ų� ��峪 Ȯ���ϰ� ����� ��� 
int isGameOver(int (*board)[BOARDSIZE], int player)
{
	char *Player[3] = {" ", "Compuer", "User"};

	if(isWin(board, player))
	{
		printf("%s won!\n", Player[player]);
	}
	else if(CheckedCount == BOARDSIZE * BOARDSIZE)
	{
		printf("Draw!\n");
	}
	else
	{
		return false;
	}
	return true;
}

// ������ ����� ��ġ������ ��ȯ�Ͽ� ��ȯ �迭�� ������ ������ �־��ش�. 
void GetEmptyPosition(int (*board)[BOARDSIZE], int *emptyPosition)
{
	int i, j, index = 0;
	for(i = 0; i < BOARDSIZE; i++)
	{
		for(j = 0; j < BOARDSIZE; j++)
		{
			if(board[i][j] == 0)
			{
				emptyPosition[index++] = i * BOARDSIZE + j;
			}
		}
	}
	emptyPosition[9] = index; //����� ������ �ʿ��ϹǷ� �迭�� �������� ǥ������ 
}

// ��ǻ�Ͱ� �� ��ġ�� ã������ �Լ� 
void SetBestPosition(int pos, int score)
{
	if(bestPosition[1] < score)
	{
		bestPosition[0] = pos;
		bestPosition[1] = score;
	}
}

// ����Ʈ ��ġ�� ã������ ��ġ�Լ� 
int Minimax(int depth, int (*board)[BOARDSIZE], int player)
{
	if(depth == 0 || isWin(board, 3- player) || CheckedCount == 9)
	{
		return evaluation();
	}
	
	int emptyPosition[10];
	GetEmptyPosition(board, emptyPosition);
	if(player == Computer)
	{
		int i, score, maxScore = -100;
		for(i = 0; i < emptyPosition[9]; i++)
		{
			SetNumber(board, emptyPosition[i], player);
			score = Minimax(depth - 1, board, User);
			SetZero(board, emptyPosition[i]);
			maxScore = max(score, maxScore);
			if(depth == maxDepth)
			{
				SetBestPosition(emptyPosition[i], maxScore);
				printf("pos = %d, score = %d\n", emptyPosition[i] + 1, maxScore);
			}
		}
		return maxScore;
	}
	else
	{
		int i, score, minScore = 100;
		for(i = 0; i < emptyPosition[9]; i++)
		{
			SetNumber(board, emptyPosition[i], player);
			score = Minimax(depth - 1, board, Computer);
			SetZero(board, emptyPosition[i]);
			minScore = min(score, minScore);
		}
		return minScore;
	}
}

// ������ ����Ǵ� �Լ� 
int PlayGame(int (*board)[BOARDSIZE], int player)
{
	if(player == Computer)
	{
		Minimax(maxDepth, board, player);
		// �� ���� �ΰ��� ����Ʈ ���� �ʱ�ȭ����� ������ �ǹٸ� ���� ã�� �� �ֽ� 
		bestPosition[1] = -1; 
		return bestPosition[0];
	}
	else
	{
		return GetNumber(board);
	}
}

// �� ���� ���Ұ������� �����ϴ� �Լ� 
int isContinue()
{
	char yesno;
	printf("One more? (y/n) : ");
	yesno = getch();
	if(yesno == 'y' || yesno == 'Y')
	{
		printf("%c\n", yesno);
		return true;
	}
	return false;
}

// ������ ���۵� �� �ʱ�ȭ�� �ʿ��� �������� ��Ƽ� �ʱ�ȭ ���� 
int InitGame(int (*board)[BOARDSIZE])
{
	int player;
	InitBoard(board);
	printf("Tic Tac Toe v1.0\n");
	PrintBoard(board);
	CheckedCount = 0;
	bestPosition[1] = -1;
	player = rand() % 2 + 1; // ���� �����ϱ� ���Ͽ� 
	return player;
}

int main(void)
{
	int i;
	int player = 0;
	int position;
	int board[BOARDSIZE][BOARDSIZE];
	
	srand((unsigned)time(NULL));
	player = InitGame(board);
	while(true) 
	{
		position = PlayGame(board, player);
		SetNumber(board, position, player);
		PrintBoard(board);
		if(isGameOver(board, player))
		{
			if(!isContinue())
			{
				break;
			}
			else
			{
				player = InitGame(board);
			}
		}
		player = 3 - player;
	}
	
	return 0;
}
