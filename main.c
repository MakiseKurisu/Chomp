#include "stdafx.h"

#define MCTS_COEFFICIENT 20	//3x3

LPCHOMP_NODE Flat(LPCHOMP_NODE Node, LPCHOMP_BOARD Board)
{
	int Index = 1;
	if (Board->PieceCount > 1)
		Index = secure_rand(2, Board->PieceCount);
	MakeMoveIndex(Board, Index);
	return CreateChildNodeFromIndex(Node, Board, Index);
}

LPCHOMP_NODE UCB1(LPCHOMP_NODE Node, LPCHOMP_BOARD Board)
{
	while (!EndState(Board) && !IsNodeEmpty(Node))
	{
		DPRINT("depth %i\n", Node->Depth);

		double * Rating = malloc(sizeof(double) * (Board->PieceCount));
		for (int i = (Board->PieceCount - 1); i > 0; i--)
		{
			Rating[i] = INFINITY;
		}

		DPRINT("init rating array %i\n", Board->PieceCount);

		LPCHOMP_NODE Child = Node->FirstChild;
		while (Child)
		{
			Rating[(Child->Index - 1)] = ((double)(Child->WinCount[Board->Turn])) / (Child->TotalCount) + sqrt(2 * log(Node->TotalCount) / (Child->TotalCount));
			DPRINT("child %i rated %f\n", Child->Index, Rating[(Child->Index - 1)]);
			Child = Child->Next;
		}

		int Index = (Board->PieceCount - 1);
		for (int i = (Board->PieceCount - 1); i > 0; i--)
		{
			if (isinf(Rating[i]))
			{
				DPRINT("child %i has no node\n", i);
				Index = i;
				break;
			}
			else if (Rating[i] > Rating[Index])
			{
				DPRINT("child %i has higher rating than %i\n", i, Index);
				Index = i;
			}
		}
		DPRINT("select %i\n", Index);
		free(Rating);
		Node = CreateChildNodeFromIndex(Node, Board, Index + 1);
		MakeMoveIndex(Board, Index + 1);
	}
	DPRINT("end selection\n\n");
	return Node;
}

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
			LPCHOMP_BOARD ChildBoard = CopyBoard(CurrentBoard);
			if (!ChildBoard)
				break;

			// Selection & Expansion
			LPCHOMP_NODE ChildNode = UCB1(CurrentNode, ChildBoard);

			// Simulation
			while (!EndState(ChildBoard))
			{
				int X, Y;
				int RandomMove = 1;
				if (ChildBoard->PieceCount > 1)
					RandomMove = secure_rand(2, ChildBoard->PieceCount);

				//*
				ChildNode = CreateChildNodeFromIndex(ChildNode, ChildBoard, RandomMove);
				if (!ChildNode)
				{
					FreeBoard(ChildBoard);
					break;
				}
				//*/

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
					ChildNode->WinCount[Player]++;
				} while ((ChildNode = ChildNode->Parent) != NULL);
			}

			// Clean up
			FreeBoard(ChildBoard);
		}

		// Make decision based on MCTS
		LPCHOMP_NODE BestNode = BestChildNode(CurrentNode, CurrentBoard);

		// Display message
		PrintChild(CurrentNode);
		printf("player %i plays at %i,%i\n", 1 + (int)CurrentBoard->Turn, BestNode->X + 1, BestNode->Y + 1);
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

