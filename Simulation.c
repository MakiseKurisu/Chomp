#include "stdafx.h"

void FreeBoard(LPCHOMP_BOARD Board)
{
	if (Board)
	{
		free(Board);
	}
}

LPCHOMP_BOARD CreateNewBoard(int Length, int Width)
{
	if (Length <= 0 || Width <= 0)
	{
		return NULL;
	}

	int PieceCount = Length * Width;
	size_t Size = sizeof(CHOMP_BOARD) + sizeof(CHOMP_PIECE) * PieceCount;

	LPCHOMP_BOARD Board = malloc(Size);
	if (Board)
	{
		Board->Size = Size;
		Board->Length = Length;
		Board->Width = Width;
		Board->PieceCount = PieceCount;
		Board->Turn = 0;
		for (int i = 0; i < PieceCount; i++)
		{
			(Board->States)[i] = PIECE_VALID;
		}
	}
	return Board;
}

LPCHOMP_BOARD CopyBoard(LPCHOMP_BOARD OldBoard)
{
	if (!OldBoard)
	{
		return NULL;
	}

	LPCHOMP_BOARD NewBoard = malloc(OldBoard->Size);
	if (NewBoard)
	{
		memcpy(NewBoard, OldBoard, OldBoard->Size);
	}
	return NewBoard;
}

bool MakeMove(LPCHOMP_BOARD Board, int X, int Y)
{
	if (!Board || X < 0 || Y < 0 || PIECE(Board, X, Y) == PIECE_INVALID)
	{
		return false;
	}

	for (int y = Y; y < Board->Width; y++)
	{
		for (int x = X; x < Board->Length; x++)
		{
			if (PIECE(Board, x, y) == PIECE_VALID)
			{
				PIECE(Board, x, y) = PIECE_INVALID;
				Board->PieceCount--;
			}
		}
	}

	if (Board->Turn == PLAYER_1)
	{
		Board->Turn = PLAYER_2;
	}
	else
	{
		Board->Turn = PLAYER_1;
	}

	return true;
}

bool MakeMoveIndex(LPCHOMP_BOARD Board, int Index)
{
	int X, Y;
	if (!IndexToCoord(Board, Index, &X, &Y))
	{
		return false;
	}
	return MakeMove(Board, X, Y);
}

bool IndexToCoord(LPCHOMP_BOARD Board, int Index, int* X, int* Y)
{
	if (!Board || !X || !Y || Index <= 0 || Index > Board->PieceCount)
	{
		return false;
	}

	for (int y = 0; y < Board->Width; y++)
	{
		for (int x = 0; x < Board->Length; x++)
		{
			if (PIECE(Board, x, y) == PIECE_VALID)
			{
				Index--;
				if (Index == 0)
				{
					*X = x;
					*Y = y;
					return true;
				}
			}
		}
	}

	return false;
}

bool CoordToIndex(LPCHOMP_BOARD Board, int* Index, int X, int Y)
{
	if (!Board || !Index || X <= 0 || Y <= 0 || X > Board->Width || Y > Board->Length)
	{
		return false;
	}

	*Index = 0;
	for (int y = 0; y < Board->Width; y++)
	{
		for (int x = 0; x < Board->Length; x++)
		{
			if (PIECE(Board, x, y) == PIECE_VALID)
			{
				*Index++;
				if (x == X && y == Y)
				{
					return true;
				}
			}
		}
	}

	return false;
}

int GetPossibleMoveCount(LPCHOMP_BOARD Board)
{
	if (Board)
	{
		return Board->PieceCount;
	}
	else
	{
		return 0;
	}
}

bool EndState(LPCHOMP_BOARD Board)
{
	if (Board)
	{
		//print(Board);
		return !(Board->PieceCount);
	}
	else
	{
		return true;
	}
}

CHOMP_PLAYER Winner(LPCHOMP_BOARD Board)
{
	if (Board && EndState(Board))
	{
		return Board->Turn;
	}
	else
	{
		return PLAYER_INVALID;
	}
}

void PrintBoard(LPCHOMP_BOARD Board)
{
	if (Board)
	{
		for (int y = (Board->Width) - 1; y >= 0; y--)
		{
			for (int x = 0; x < Board->Length; x++)
			{
				if (PIECE(Board, x, y) == PIECE_INVALID)
				{
					break;
				}
				else
				{
					printf("O");
				}
			}
			if (PIECE(Board, 0, y) == PIECE_VALID)
			{
				printf("\n");
			}
		}
	}
}