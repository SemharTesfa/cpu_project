#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "project.h"

/**
 * Authors: Semhar Teklehaimanot and Vinhem Duong
 * Date: August 8, 2024
 * Description: This function represents the process manager which reads commands
 * from the pipe and executes the corresponding functions such as running a quantum,
 * unblocking processes, printing the system state, or terminating the process manager.
 */

void process_manager(int readPipe) {
    init_system();

    // Load initial program
    load_initial_program();

    char ch;
    do {
        if (read(readPipe, &ch, sizeof(ch)) != sizeof(ch)) {
            perror("read");
            break;
        }

        switch (ch) {
            case 'Q':
                quantum(); // Run a quantum
                break;
            case 'U':
                //Places blocked processes into ready queue
                unblock();
                break;
            case 'P':
                print_system_state(); // Print the system state
                break;
            case 'T':
                printf("Terminate command received.\n");
                break;
            default:
                printf("Invalid command: %c\n", ch);
                break;
        }
    } while (ch != 'T'); // Terminate the process manager
}