#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include "project.h"


/**
 * Authors: Semhar Teklehaimanot and Vinhem Duong
 * Date: August 8, 2024
 * Description: This function represents the commander process which reads commands
 * from the user and writes them to the pipe for the process manager to execute.
 */


void commander_process(int writePipe) {
    char ch;
    do {
        // Prompt the user for a command
        printf("Enter Q, P, U, or T: $\n");
        scanf(" %c", &ch);
        ch = toupper(ch);

        // Write the command to the pipe
        if (write(writePipe, &ch, sizeof(ch)) != sizeof(ch)) {
            perror("write");
            break;
        }
    } while (ch != 'T'); // Terminate the commander process
}