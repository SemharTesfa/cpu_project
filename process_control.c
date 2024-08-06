#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include "project.h"

/**
 * Authors: Semhar Teklehaimanot and Vinhem Duong
 * Date: August 8, 2024
 * Description: This code simulates a basic process scheduler with capabilities to handle running, ready, and blocked states.
 * The code includes functionalities to initialize the system, load programs, execute instructions, and print the current state of the system.
 */

// Initialize the system 
void init_system() {
    memset(pcbTable, 0, sizeof(pcbTable));
    memset(readyState, 0, sizeof(readyState));
    memset(blockedState, 0, sizeof(blockedState));
    timestamp = 0;
    runningState = -1;
    readyCount = 0;
    blockedCount = 0;
    cumulativeTimeDiff = 0;
    numTerminatedProcesses = 0;
}

// Load the initial program into the PCB table
void load_initial_program() {
    Instruction program[MAX_INSTRUCTIONS] = {
        {'S', 1000, ""},
        {'A', 19, ""},
        {'A', 20, ""},
        {'D', 53, ""},
        {'A', 55, ""},
        {'F', 1, ""},
        {'R', 5, "file_a.txt"},
        {'B', 1, ""},
        {'F', 2, ""},
        {'R', 1, "file_b.txt"},
        {'F', 3, ""},
        {'R', 2, "file_c.txt"},
        {'F', 4, ""},
        {'R', 0, "file_d.txt"},
        {'F', 5, ""},
        {'R', 1, "file_e.txt"},
        {'E', 0, ""}
    };


    int programLength = sizeof(program) / sizeof(program[0]);
    for (int i = 0; i < programLength; i++) {
        pcbTable[0].program[i] = program[i];
    }

    pcbTable[0].processId = 0;
    pcbTable[0].parentProcessId = -1; // Assuming the initial process has no parent
    pcbTable[0].programCounter = 0;
    pcbTable[0].value = 0; // Initial value set to 0
    pcbTable[0].priority = 0;
    pcbTable[0].state = STATE_RUNNING;
    pcbTable[0].startTime = 0;
    pcbTable[0].timeUsed = 0;

    runningState = 0;

    cpu.programCounter = pcbTable[0].programCounter;
    cpu.value = pcbTable[0].value;

    timestamp = 0;
}


// Execute a quantum of the processing time
void quantum() {
    Instruction instruction;

    if (runningState == -1) {
        printf("No processes are running.\n");
        timestamp++;
        return;
    }

    if (cpu.programCounter < MAX_INSTRUCTIONS && pcbTable[runningState].program[cpu.programCounter].operation != 0) {
        instruction = pcbTable[runningState].program[cpu.programCounter];
        cpu.programCounter++;
    } else {
        printf("End of program reached without E operation.\n");
        instruction.operation = 'E';
    }

    printf("Executing instruction: %c %d %s\n", instruction.operation, instruction.intArg, instruction.stringArg);  // Debug info

    switch (instruction.operation) {
        case 'S':
            set(instruction.intArg);
            break;
        case 'A':
            add(instruction.intArg);
            break;
        case 'D':
            decrement(instruction.intArg);
            break;
        case 'B':
            block();
            break;
        case 'E':
            end_process();
            break;
        case 'F':
            fork_process(instruction.intArg);
            break;
        case 'R':
            makeInstruction(instruction.stringArg);
            break;
        default:
            printf("Unknown operation: %c\n", instruction.operation);
            break;
    }

    timestamp++;
    schedule();
}


// block the current process
void block() {
    if (runningState == -1) return;

    printf("Blocking process: %d\n", pcbTable[runningState].processId);

    blockedState[blockedCount++] = runningState;
    pcbTable[runningState].state = STATE_BLOCKED;
    pcbTable[runningState].programCounter = cpu.programCounter;
    pcbTable[runningState].value = cpu.value;
    runningState = -1;

    schedule();
}

// End the current process
void end_process() {
    if (runningState == -1) return;

    printf("Ending process: %d\n", pcbTable[runningState].processId);

    cumulativeTimeDiff += timestamp - pcbTable[runningState].startTime;
    numTerminatedProcesses++;
    pcbTable[runningState].state = STATE_TERMINATED;
    runningState = -1;

    schedule();
}

// Fork a new process fro the running process
void fork_process(int value) {
    if (runningState == -1 || value >= MAX_PROCESSES) return;

    // Find an unused process ID
    int newProcessId = -1;
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (pcbTable[i].state == STATE_TERMINATED || pcbTable[i].state == STATE_NEW) {
            newProcessId = i;
            break;
        }
    }

    if (newProcessId == -1) {
        printf("No available process ID for forking.\n");
        return;
    }

    // Initialize the new process
    pcbTable[newProcessId].processId = rand() % 100000000;
    pcbTable[newProcessId].parentProcessId = pcbTable[runningState].processId;
    memcpy(pcbTable[newProcessId].program, pcbTable[runningState].program, sizeof(pcbTable[runningState].program));
    pcbTable[newProcessId].programCounter = 0;
    pcbTable[newProcessId].value = cpu.value;
    pcbTable[newProcessId].priority = value;
    pcbTable[newProcessId].state = STATE_READY;
    pcbTable[newProcessId].startTime = timestamp;
    pcbTable[newProcessId].timeUsed = 0;

    // Add the new process to the ready queue
    readyState[readyCount++] = newProcessId;

    printf("Forking process: %d\n", newProcessId);

    schedule();
}

// unblock a process from the blocked queue
void unblock() {
    if (blockedCount > 0) {
        int unblockedProcess = blockedState[0];

        // Shift blocked queue
        for (int i = 1; i < blockedCount; i++) {
            blockedState[i - 1] = blockedState[i];
        }
        blockedCount--;

        readyState[readyCount++] = unblockedProcess;
        pcbTable[unblockedProcess].state = STATE_READY;

        printf("Unblocking process: %d\n", pcbTable[unblockedProcess].processId);

        schedule();
    }
}

// Schedule the next process to run
void schedule() {
    if (runningState != -1) 
        return;

    if (readyCount > 0) {
        runningState = readyState[0];

        // Shift ready queue
        for (int i = 1; i < readyCount; i++) {
            readyState[i - 1] = readyState[i];
        }
        readyCount--;

        pcbTable[runningState].state = STATE_RUNNING;
        cpu.programCounter = pcbTable[runningState].programCounter;
        cpu.value = pcbTable[runningState].value;

        printf("Scheduled process: %d\n", pcbTable[runningState].processId);
    }
}

// Print the current state of the system
void print_system_state() {
    // Print current timestamp
    printf("\n+-----------------------+--------------+---------------------------------------------------------------------------+\n");
    printf("| %-21s | %-12u |\n", "Current timestamp", timestamp);
    printf("+-----------------------+--------------+---------------------------------------------------------------------------+\n");

    // Print the summary table header
    printf("| %-21s | %-12s | %-12s | %-12s | %-12s | %-12s | %-12s |\n", 
            "State", "PID", "PPID", "Priority", "Value", "Start Time", "CPU Time Used");
    printf("+-----------------------+--------------+--------------+--------------+--------------+--------------+---------------+\n");

    // Print running state summary
    printf("| %-21s | %-12s | %-12s | %-12s | %-12s | %-12s | %-12s |\n", 
                "Running State", "", "", "", "", "", "");
    if (runningState != -1) { // Assuming -1 means no running process
        printf("| %-21s | %-12d | %-12d | %-12d | %-12d | %-12d | %-12d |\n", 
                "", pcbTable[runningState].processId, pcbTable[runningState].parentProcessId, 
                pcbTable[runningState].priority, cpu.value, 
                pcbTable[runningState].startTime, timestamp - pcbTable[runningState].startTime);
    } else {
        printf("| %-21s | %-12s | %-12s | %-12s | %-12s | %-12s | %-12s |\n", 
                "", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A");
    }
    printf("+-----------------------+--------------+--------------+--------------+--------------+--------------+---------------+\n");

    // Print ready queue summary
    printf("| %-21s | %-12s | %-12s | %-12s | %-12s | %-12s | %-12s |\n", 
                "Ready Queue", "", "", "", "", "", "");
    for (int i = 0; i < readyCount; i++) {
        printf("| %-21s | %-12d | %-12d | %-12d | %-12d | %-12d | %-12d |\n", 
                "", pcbTable[readyState[i]].processId, pcbTable[readyState[i]].parentProcessId, 
                pcbTable[readyState[i]].priority, pcbTable[readyState[i]].value, 
                pcbTable[readyState[i]].startTime, pcbTable[readyState[i]].timeUsed);
    }
    printf("+-----------------------+--------------+--------------+--------------+--------------+--------------+---------------+\n");

    // Print blocked queue summary
    printf("| %-21s | %-12s | %-12s | %-12s | %-12s | %-12s | %-12s |\n", 
                "Blocked Queue", "", "", "", "", "", "");
    for (int i = 0; i < blockedCount; i++) {
        printf("| %-21s | %-12d | %-12d | %-12d | %-12d | %-12d | %-12d |\n", 
                "", pcbTable[blockedState[i]].processId, pcbTable[blockedState[i]].parentProcessId, 
                pcbTable[blockedState[i]].priority, pcbTable[blockedState[i]].value, 
                pcbTable[blockedState[i]].startTime, pcbTable[blockedState[i]].timeUsed);
    }
    printf("+-----------------------+--------------+--------------+--------------+--------------+--------------+---------------+\n");
}

// load a program from a file into the insturction array
bool load_program_from_file(char filename[], Instruction *program, int *programLength) {
    FILE *file = fopen(filename, "r");
    
    if (!file) {
        perror("fopen");
        return false;
    }

    char line[256];
    int index = 0;

    while (fgets(line, sizeof(line), file)) {
        if (index >= MAX_INSTRUCTIONS) {
            printf("Program too long, truncating to %d instructions\n", MAX_INSTRUCTIONS);
            break;
        }

        char operation;
        int intArg = 0;
        char stringArg[256] = {0};

        int itemsParsed = sscanf(line, "%c %d %255s", &operation, &intArg, stringArg);
        if (itemsParsed > 0) {
            program[index].operation = operation;
            program[index].intArg = intArg;
            strncpy(program[index].stringArg, stringArg, sizeof(program[index].stringArg) - 1);
            index++;
        }
    }

    *programLength = index;
    fclose(file);
    return true;
}
