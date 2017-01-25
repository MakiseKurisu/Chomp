#pragma once

#include "targetver.h"

#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h> 
#include <stdio.h>
#include <tchar.h>
#include <stdbool.h>
#include <math.h>
#include "Simulation.h"
#include "GameTree.h"
#include "rand.h"

#ifdef _DEBUG
#define DPRINT(...) printf(##__VA_ARGS__)
#else
#define DPRINT(...)
#endif