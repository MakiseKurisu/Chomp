#include "stdafx.h"

#define MCTS_COEFFICIENT 1024

void StartMCTS(int Length, int Width, int Iteration)
{
	LPCHOMP_NODE RootNode = CreateRootNode();
	if (!RootNode)
	{
		printf("Cannot create root node.\n");
		return;
	}
	LPCHOMP_NODE CurrentNode = RootNode;

	LPCHOMP_BOARD CurrentBoard = CreateNewBoard(Length, Width);
	if (!CurrentBoard)
	{
		printf("Cannot create root board.\n");
		return;
	}

	printf("Successfully initialized with %ix%i board.\n", Length, Width);

	while (!EndState(CurrentBoard))
	{
		for (int i = 0; i < (CurrentBoard->PieceCount * Iteration); i++)
		{
			// Selection
			int ChildIndex = 1;
			if (CurrentBoard->PieceCount > 1)
				ChildIndex = secure_rand(2, CurrentBoard->PieceCount);

			// Expansion
			LPCHOMP_NODE ChildNode = CreateChildNodeFromIndex(CurrentNode, CurrentBoard, ChildIndex);
			if (!ChildNode)
				break;

			// Simulation
			LPCHOMP_BOARD ChildBoard = CopyBoard(CurrentBoard);
			if (!ChildBoard)
				break;
			// Make move according to node
			if (!MakeMove(ChildBoard, ChildNode->X, ChildNode->Y))
			{
				FreeBoard(ChildBoard);
				break;
			}

			while (!EndState(ChildBoard))
			{
				int X, Y;
				int RandomMove = 1;
				if (ChildBoard->PieceCount > 1)
					RandomMove = secure_rand(2, ChildBoard->PieceCount);

				ChildNode = CreateChildNodeFromIndex(ChildNode, ChildBoard, RandomMove);
				if (!ChildNode)
				{
					FreeBoard(ChildBoard);
					break;
				}

				if (!IndexToCoord(ChildBoard, RandomMove, &X, &Y))
				{
					FreeBoard(ChildBoard);
					break;
				}
				if (!MakeMove(ChildBoard, X, Y))
				{
					FreeBoard(ChildBoard);
					break;
				}
			}

			// Backpropagation
			CHOMP_PLAYER Player = Winner(ChildBoard);
			if (Player == PLAYER_INVALID)
			{
				FreeBoard(ChildBoard);
				break;
			}
			else
			{
				do
				{
					ChildNode->TotalCount++;
					ChildNode->WinCount[(int)Player]++;
				} while ((ChildNode = ChildNode->Parent) != NULL);
			}

			// Clean up
			FreeBoard(ChildBoard);
		}

		// Make decision based on MCTS
		LPCHOMP_NODE BestNode = BestChildNode(CurrentNode, CurrentBoard->Turn);

		// Display message
		//PrintChild(CurrentNode);
		//printf("player %i plays at %i,%i\n", 1 + (int)CurrentBoard->Turn, BestNode->X + 1, BestNode->Y + 1);
		//PrintChild(BestNode);

		// Update board and root node
		MakeMove(CurrentBoard, BestNode->X, BestNode->Y);
		CurrentNode = BestNode;
		//BestNode->WinCount[0] = BestNode->WinCount[1] = BestNode->TotalCount = 0;

		// Print board
		PrintBoard(CurrentBoard);
		printf("\n");
	}

	switch (Winner(CurrentBoard))
	{
	case PLAYER_1:
	{
		printf("The winner is player 1.\n\n");
		break;
	}
	case PLAYER_2:
	{
		printf("The winner is player 2.\n\n");
		break;
	}
	default:
	{
		printf("Simulation ended with error.\n\n");
		break;
	}
	}

	FreeBoard(CurrentBoard);
	FreeNode(RootNode);
}

int main()
{
	while (true)
	{
		int Length = 0, Width = 0;
		printf("Please enter the dimension of the board in format x,y: ");
		scanf_s("%i,%i", &Length, &Width);
		if (Length == 0)
		{
			printf("Invalid board length.\n");
			continue;
		}
		else if (Width == 0)
		{
			printf("Invalid board width.\n");
			continue;
		}
		else if (Length < 0 || Width < 0)
		{
			break;
		}
		StartMCTS(Length, Width, MCTS_COEFFICIENT);
	}
	_CrtDumpMemoryLeaks();
	return 0;
}

