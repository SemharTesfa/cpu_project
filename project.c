#include "project.h"

/**
 * Authors: Semhar Teklehaimanot and Vinhem Duong
 * Date: August 8, 2024
 * Description: Implementation file for the process management simulation project.
 * It defines the global variables used in the project and initializes them.
 */

// Define the global variables
PcbEntry pcbTable[MAX_PROCESSES];
unsigned int timestamp = 0;
Cpu cpu;
int runningState = -1;
//Should be a deque
int readyState[MAX_PROCESSES];
int blockedState[MAX_PROCESSES];
//Deque readyProcess;
//Deque blockProcess;
int readyCount = 0;
int blockedCount = 0;
double cumulativeTimeDiff = 0;
int numTerminatedProcesses = 0;
