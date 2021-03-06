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

int CheckedCount = 0; //이미 둔 자리의 수 9가 되면 비긴 것 
int winner = 0; //Computer = 1, User = 2
int maxDepth = 9; //몇 수 앞까지 두어볼지를 결정 
int bestPosition[2] = {-1, -1}; //구조체로 할까 하다 간단하게 배열로 사용 Mimimax 함수에서 사용 

//1차원 숫자(1-9)로 입력된 값을 좌표 값으로 변환하기 위한 구조체 
typedef struct tagPoint{
	int row;
	int col;
}Point;

// 1~9까지의 숫자를 3X3 2차원 배열에서 좌표로 변환 
Point GetPoint(int position)
{
	Point p;
	p.row = position / BOARDSIZE;
	p.col = position % BOARDSIZE;
	return p;
}

// 입력이 1차원이고, 보드는 2차원으로 되어 있어 입력된 위치에 체크되어 있는지 검사 
int isFill(int (*board)[BOARDSIZE], int position)
{
	Point p = GetPoint(position);
	if(board[p.row][p.col])
	{
		return true;
	}
	return false;
}

// 보드에 체크할 위치를 숫자로 입력받기위한 함수 
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

// 보드를 0으로 초기화 
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

// 보드의 상태를 화면에 표시 
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

// User 또는 Computer로부터 입력된 값을 보드에 저장 
void SetNumber(int (*board)[BOARDSIZE], int position, int player)
{
	Point p = GetPoint(position);
	board[p.row][p.col] = player;
	CheckedCount++;
}

// 주어진 위치를 0으로 복원  
void SetZero(int (*board)[BOARDSIZE], int position)
{
	Point p = GetPoint(position);
	board[p.row][p.col] = 0;
	CheckedCount--;
}

// 승리체크 
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

// 게임이 끝이 났을 때의 점수  
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

// 누군가 승리를 했거나 비겼나 확인하고 결과를 출력 
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

// 보드의 빈곳을 위치값으로 변환하여 반환 배열의 끝에는 개수를 넣어준다. 
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
	emptyPosition[9] = index; //빈곳의 개수가 필요하므로 배열의 마지막에 표시해줌 
}

// 컴퓨터가 둘 위치를 찾기위한 함수 
void SetBestPosition(int pos, int score)
{
	if(bestPosition[1] < score)
	{
		bestPosition[0] = pos;
		bestPosition[1] = score;
	}
}

// 베스트 위치를 찾기위한 서치함수 
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

// 게임이 진행되는 함수 
int PlayGame(int (*board)[BOARDSIZE], int player)
{
	if(player == Computer)
	{
		Minimax(maxDepth, board, player);
		// 한 수를 두고나면 베스트 값을 초기화해줘야 다음에 옳바른 값을 찾을 수 있슴 
		bestPosition[1] = -1; 
		return bestPosition[0];
	}
	else
	{
		return GetNumber(board);
	}
}

// 한 게임 더할것인지를 결정하는 함수 
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

// 게임이 시작될 때 초기화가 필요한 변수들을 모아서 초기화 해줌 
int InitGame(int (*board)[BOARDSIZE])
{
	int player;
	InitBoard(board);
	printf("Tic Tac Toe v1.0\n");
	PrintBoard(board);
	CheckedCount = 0;
	bestPosition[1] = -1;
	player = rand() % 2 + 1; // 선을 선택하기 위하여 
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
