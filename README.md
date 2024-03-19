# MyShell
# Command Shell

This is a simple command shell implemented in C that runs on Linux systems. It allows users to interactively execute built-in Linux commands along with some additional features. The shell runs an infinite loop, processing user commands until the 'exit' command is entered.

## Features

1. **Basic Functionality**: The shell provides a prompt indicating the current working directory, allowing users to enter commands interactively. It utilizes `getline()` and `strsep()` functions for input processing and command parsing.

2. **Changing Directory**: Supports the `cd` command to change the working directory. Users can use `cd <directoryPath>` to change to a specific directory and `cd ..` to move to the parent directory.

3. **Incorrect Command Handling**: Displays an error message ('Shell: Incorrect command') when the shell encounters an unknown command format. Additionally, it displays any error messages generated during command execution.

4. **Signal Handling**: Handles signals generated from the keyboard using 'Ctrl + C' and 'Ctrl + Z'. The shell continues to function normally, and the commands being executed respond to these signals. The shell only exits with the 'exit' command.

5. **Executing Multiple Commands**: Supports executing multiple commands sequentially or in parallel. Commands separated by '&&' are executed in parallel, while commands separated by '##' are executed sequentially. The shell waits for all commands to terminate before accepting further inputs.

6. **Output Redirection**: Capable of redirecting standard output (STDOUT) using the '>' symbol. For example, `ls > info.out` writes the output of the `ls` command to the 'info.out' file instead of displaying it on the screen.

7. **Support for Command Pipelines (Optional)**: Provides support for command pipelines using the '|' token. Users can execute command pipelines such as `cat myshell.c | grep open | wc`. This feature enables redirecting the standard output of one command to the standard input of another command.

## Usage

To use the command shell, follow these steps:

1. Compile the provided source code (`myshell.c`) using a C compiler on a Linux system.
   
2. Run the compiled executable to start the shell.

3. Enter commands interactively at the prompt. Use 'exit' to terminate the shell.

## Implementation Details

The shell source code is modular and well-commented for easy understanding. It utilizes system calls such as `fork()`, `exec()`, `wait()`, and `chdir()` to execute commands and manage processes. Signal handling is implemented to ensure the shell responds appropriately to keyboard signals.

