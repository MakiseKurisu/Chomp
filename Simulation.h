#pragma once

typedef enum
{
	PLAYER_1 = 0,
	PLAYER_2 = 1,
	PLAYER_INVALID = 2,
} CHOMP_PLAYER;
#define CHOMP_MAX_PLAYER 2

#define PIECE_INVALID	0
#define PIECE_VALID		1

typedef char CHOMP_PIECE, *LPCHOMP_PIECE;

#define PIECE(board, x, y) ((board->States)[x + y * board->Length])

typedef struct {
	size_t Size;
	int Length;
	int Width;
	int PieceCount;
	CHOMP_PLAYER Turn;
	CHOMP_PIECE States[0];
} CHOMP_BOARD, *LPCHOMP_BOARD;

void FreeBoard(LPCHOMP_BOARD Board);
LPCHOMP_BOARD CreateNewBoard(int Length, int Width);
LPCHOMP_BOARD CopyBoard(LPCHOMP_BOARD OldBoard);
bool MakeMove(LPCHOMP_BOARD Board, int X, int Y);
bool IndexToCoord(LPCHOMP_BOARD Board, int Index, int* X, int* Y);
bool CoordToIndex(LPCHOMP_BOARD Board, int* Index, int X, int Y);
int GetPossibleMoveCount(LPCHOMP_BOARD Board);
bool EndState(LPCHOMP_BOARD Board);
CHOMP_PLAYER Winner(LPCHOMP_BOARD Board);
void PrintBoard(LPCHOMP_BOARD Board);