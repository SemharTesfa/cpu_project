# Process Management Simulation Project

## Description

This project simulates a basic process management system. It involves two main processes: the commander process and the process manager process. The commander process reads user commands and sends them to the process manager through a pipe. The process manager executes these commands to manage the state of various processes.

## Authors

- Semhar Teklehaimanot
- Vinhem Duong

## Date

- August 8, 2024

## Features

- Initialize the system state.
- Load initial programs into the system.
- Execute CPU operations such as setting, adding, and decrementing values.
- Manage process states including running, ready, blocked, and terminated.
- Print the current state of the system.
- Handle user commands to control the process flow.

## Commands

- `Q`: Execute a quantum of processing time.
- `U`: Unblock processes from the blocked queue and move them to the ready queue.
- `P`: Print the current state of the system.
- `T`: Terminate the process manager.

## Usage

1. Clone the repository to your local machine:

    ```sh
    git clone https://github.com/yourusername/process-management-simulation.git
    cd process-management-simulation
    ```

2. Compile the project:

    ```sh
    gcc main.c commander.c process_manager.c cpu_operations.c process_control.c project.c -o project
    ```

3. Run the project:

    ```sh
    ./project
    ```

4. Follow the on-screen prompts to enter commands (`Q`, `P`, `U`, or `T`).

## File Structure

- `main.c`: Contains the main function to initialize and run the commander and process manager processes.
- `commander.c`: Implements the commander process that reads user commands.
- `process_manager.c`: Implements the process manager process that executes commands.
- `cpu_operations.c`: Implements CPU operations such as setting, adding, and decrementing values.
- `process_control.c`: Implements process control functions like blocking, unblocking, forking, and ending processes.
- `project.c`: Implements utility functions and defines global variables.
- `project.h`: Header file defining constants, data structures, and function prototypes.

## Function Descriptions

### Commander Process

The commander process reads user commands and sends them to the process manager.

```c
void commander_process(int writePipe);

### Process Manager
```c
void process_manager(int readPipe);

### Commander Process

```c
void set(int value);
void add(int value);
void decrement(int value);

### Commander Process

```c
void block();
void end_process();
void fork_process(int value);
void unblock();
void schedule();
void print_system_state();
bool load_program_from_file(char stringArg[], Instruction *program, int *programLength);




