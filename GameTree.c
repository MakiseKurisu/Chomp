#include "stdafx.h"

bool IsNodeEmpty(LPCHOMP_NODE Node)
{
	return (Node->TotalCount == 0);
}

void FreeNode(LPCHOMP_NODE Node)
{
	if (Node)
	{
		LPCHOMP_NODE Child = Node->FirstChild;
		while (Child)
		{
			LPCHOMP_NODE Next = Child->Next;
			FreeNode(Child);
			Child = Next;
		}

		free(Node);
	}
}

LPCHOMP_NODE CreateRootNode()
{
	LPCHOMP_NODE Root = malloc(sizeof(CHOMP_NODE));
	if (Root)
	{
		Root->Parent = Root->FirstChild = Root->LastChild = Root->Next = NULL;
		Root->Depth = 0;
		Root->X = Root->Y = Root->Index = -1;
		Root->TotalCount = 0;
		for (int i = 0; i < CHOMP_MAX_PLAYER; i++)
		{
			Root->WinCount[i] = 0;
		}

	}
	return Root;
}

LPCHOMP_NODE SearchChildNode(LPCHOMP_NODE Node, int X, int Y)
{
	if (Node)
	{
		LPCHOMP_NODE Child = Node->FirstChild;
		while (Child)
		{
			if (X == Child->X && Y == Child->Y)
			{
				return Child;
			}
			else
			{
				Child = Child->Next;
			}
		}
	}
	return NULL;
}

LPCHOMP_NODE CreateChildNode(LPCHOMP_NODE Parent, int X, int Y, int Index)
{

	LPCHOMP_NODE Child = SearchChildNode(Parent, X, Y);
	if (Child)
	{
		return Child;
	}
	else
	{
		Child = malloc(sizeof(CHOMP_NODE));
	}

	if (Child)
	{
		Child->Parent = Parent;
		Child->FirstChild = Child->LastChild = NULL;
		Child->Next = NULL;
		Child->Depth = Parent->Depth + 1;
		Child->X = X;
		Child->Y = Y;
		Child->Index = Index;
		Child->TotalCount = 0;
		for (int i = 0; i < CHOMP_MAX_PLAYER; i++)
		{
			Child->WinCount[i] = 0;
		}

		if (!Parent->FirstChild)
		{
			Parent->FirstChild = Parent->LastChild = Child;
		}
		else
		{
			Parent->LastChild->Next = Child;
			Parent->LastChild = Child;
		}
	}
	return Child;
}

LPCHOMP_NODE CreateChildNodeFromIndex(LPCHOMP_NODE Parent, LPCHOMP_BOARD Board, int Index)
{
	int X, Y;
	if (IndexToCoord(Board, Index, &X, &Y))
	{
		return CreateChildNode(Parent, X, Y, Index);
	}
	else
	{
		return NULL;
	}
}

void PrintChild(LPCHOMP_NODE Parent)
{
	if (Parent)
	{
		LPCHOMP_NODE CurrentNode = Parent->FirstChild;
		while (CurrentNode)
		{
			printf("win ratio for %i,%i(%i):%i:%i\n", CurrentNode->X + 1, CurrentNode->Y + 1, CurrentNode->TotalCount, CurrentNode->WinCount[0], CurrentNode->WinCount[1]);
			CurrentNode = CurrentNode->Next;
		}
	}
}

LPCHOMP_NODE BestChildNode(LPCHOMP_NODE Parent, LPCHOMP_BOARD Board)
{
	if (!Parent || !Board)
	{
		return NULL;
	}

	LPCHOMP_NODE BestNode = Parent->FirstChild;
	if (!BestNode)
	{
		return NULL;
	}
	//double BestRating = ((double)(BestNode->WinCount[Board->Turn])) / (BestNode->TotalCount) + sqrt(2 * log(Parent->TotalCount) / (BestNode->TotalCount));
	int BestRating = BestNode->TotalCount;

	LPCHOMP_NODE CurrentNode = BestNode->Next;
	int i = Board->Turn;
	while (CurrentNode)
	{
		//double CurrentRating = ((double)(CurrentNode->WinCount[Board->Turn])) / (CurrentNode->TotalCount) + sqrt(2 * log(Parent->TotalCount) / (CurrentNode->TotalCount));
		int CurrentRating = CurrentNode->TotalCount;
		if (CurrentRating > BestRating)
		{
			BestNode = CurrentNode;
			BestRating = CurrentRating;
		}
		CurrentNode = CurrentNode->Next;
	}

	return BestNode;
}