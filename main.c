#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include "project.h"

/**
 * Authors: Semhar Teklehaimanot and Vinhem Duong
 * Date: August 8, 2024
 * Description: This code simulates a basic process scheduler using a commander process
 * and a process manager process. It uses a pipe for inter-process communication
 * between the commander and the process manager.
 */

int main() {
    srand(time(NULL)); // Seed the random number generator
    
    int pipeDescriptors[2];
    pid_t processMgrPid;
    int result;

    // Create a pipe
    if (pipe(pipeDescriptors) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Fork the process
    if ((processMgrPid = fork()) == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (processMgrPid == 0) {
        // The process manager process
        close(pipeDescriptors[1]);
        process_manager(pipeDescriptors[0]);
        close(pipeDescriptors[0]);
        _exit(EXIT_SUCCESS);
    } else {
        // The commander process
        close(pipeDescriptors[0]);
        commander_process(pipeDescriptors[1]);
        close(pipeDescriptors[1]);
        wait(&result);
    }

    return result;
}