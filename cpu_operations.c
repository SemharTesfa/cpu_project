#include <string.h>
#include <stdio.h>
#include "project.h"

/**
 * Authors: Semhar Teklehaimanot and Vinhem Duong
 * Date: August 8, 2024
 * Description: This file contains implementations of various CPU operations such as setting,
 * adding, and decrementing CPU values. It also includes functions to handle instructions
 * from a file and to replace the current program with a new one.
 */

extern Cpu cpu;
extern PcbEntry pcbTable[];

// set the cpu value
void set(int value) {
    cpu.value = value;
    printf("CPU set to %d\n", value);
}

// add the value to the cpu value
void add(int value) {
    cpu.value += value;
    printf("CPU added by %d, new value %d\n", value, cpu.value);
}

// decrement the value from the cpu value
void decrement(int value) {
    cpu.value -= value;
    printf("CPU decremented by %d, new value %d\n", value, cpu.value);
}


void makeInstruction(char stringArg[]) {
    FILE *file;

    file = fopen(stringArg, "r");

    //Instruction programs[MAX_INSTRUCTIONS];

    if (file != NULL) {
        char line[256];
        char command;
        int value;
        //char filename[256];

        while (fgets(line, sizeof(line), file) != NULL) {
           if (sscanf(line, "%c %d ", &command, &value) == 2) {
                //Instruction program;
                switch(command) {
                    case 'A':
                        add(value);
                        break;
                    case 'S':
                        //program.intArg = value;
                        //program.operation = 'S';
                        set(value);
                        break;
                    case 'E':
                        end_process();
                        //program.operation = 'E';
                        break;
                    default:
                        break;
                }
           } 
           //else {
           //    perror("Cannot read the line!");
           //}  
        }
    } 
    else {
        perror("Cannot read the file!");
    } 
    fclose(file);
}

// replace the current program with a new program
void replace(char argument[]) {
    Instruction newProgram[MAX_INSTRUCTIONS];
    int newProgramLength;
    
    if (load_program_from_file(argument, newProgram, &newProgramLength)) {
        memset(cpu.program, 0, sizeof(cpu.program)); // Clear the cpu program
        memcpy(cpu.program, newProgram, newProgramLength * sizeof(Instruction)); 
        cpu.programCounter = 0;
        printf("Replaced program with file: %s\n", argument);

    } else {

        printf("Failed to load program from file: %s\n", argument);
    }
}