#ifndef PROJECT_H
#define PROJECT_H
#include <stdbool.h>

/**
 * Authors: Semhar Teklehaimanot and Vinhem Duong
 * Date: August 8, 2024
 * Description: Header file for the process management simulation project. It defines
 * the constants, data structures, and function prototypes used in the project.
 */

#define MAX_INSTRUCTIONS 100
#define MAX_PROCESSES 10

// Enum to represent the state of a process
typedef enum { STATE_NEW, STATE_READY, STATE_RUNNING, STATE_BLOCKED, STATE_TERMINATED } State;

typedef struct {
    char operation;
    int intArg;
    char stringArg[256];
} Instruction;

// Struct to represent the CPU
typedef struct {
    Instruction program[MAX_INSTRUCTIONS];
    int programCounter;
    int value;
    int timeSlice;
    int timeSliceUsed;
} Cpu;

// Struct to represent a PCB entry
typedef struct {
    int processId;
    int parentProcessId;
    Instruction program[MAX_INSTRUCTIONS];
    unsigned int programCounter;
    int value;
    unsigned int priority;
    State state;
    unsigned int startTime;
    unsigned int timeUsed;
} PcbEntry;

// Declare the global variables as extern
extern PcbEntry pcbTable[MAX_PROCESSES];
extern unsigned int timestamp;
extern Cpu cpu;
extern int runningState;
extern int readyState[MAX_PROCESSES];
extern int blockedState[MAX_PROCESSES];
extern int readyCount;
extern int blockedCount;
extern double cumulativeTimeDiff;
extern int numTerminatedProcesses;

// Function Prototypes
void init_system();
void load_initial_program();
void makeInstruction(char stringArg[]);
void commander_process(int writePipe);
void process_manager(int readPipe);
void quantum();
void set(int value);
void add(int value);
void decrement(int value);
void block();
void end_process();
void fork_process(int value);
void replace(char stringArg[]);
void unblock();
void schedule();
void print_system_state();
bool load_program_from_file(char stringArg[], Instruction *program, int *programLength);

#endif // PROJECT_H
