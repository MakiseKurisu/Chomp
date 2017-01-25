#pragma once

typedef struct _CHOMP_NODE {
	struct _CHOMP_NODE *Parent;

	struct _CHOMP_NODE *FirstChild;
	struct _CHOMP_NODE *LastChild;

	struct _CHOMP_NODE *Next;

	int Depth;

	int Index;
	int X;
	int Y;
	
	int TotalCount;
	int WinCount[CHOMP_MAX_PLAYER];
} CHOMP_NODE, *LPCHOMP_NODE;

bool IsNodeEmpty(LPCHOMP_NODE Node);
void FreeNode(LPCHOMP_NODE Node);
LPCHOMP_NODE CreateRootNode();
LPCHOMP_NODE SearchChildNode(LPCHOMP_NODE Node, int X, int Y);
LPCHOMP_NODE CreateChildNode(LPCHOMP_NODE Parent, int X, int Y, int Index);
LPCHOMP_NODE CreateChildNodeFromIndex(LPCHOMP_NODE Parent, LPCHOMP_BOARD Board, int Index);
LPCHOMP_NODE CreateChildNodeFromCoord(LPCHOMP_NODE Parent, LPCHOMP_BOARD Board, int X, int Y);
LPCHOMP_NODE BestChildNode(LPCHOMP_NODE Parent, LPCHOMP_BOARD Board);
void PrintChild(LPCHOMP_NODE Parent);