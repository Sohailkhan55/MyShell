## MyShell (Custom Shell)

### Introduction
MyShell is a custom shell implementation written in C. It provides a user-friendly interface to execute various shell commands with additional features such as sequential and parallel execution, output redirection, command piping, and signal handling.

### Key Features
- **Basic Shell Commands:** Execute common shell commands like ls, cd, mkdir, etc.
- **Sequential Execution:** Run multiple commands sequentially using the `##` delimiter.
- **Parallel Execution:** Execute multiple commands in parallel using the `&&` delimiter.
- **Output Redirection:** Redirect the output of a command to a specified file using `>`.
- **Command Piping:** Execute a pipeline of commands using the `|` operator for complex operations.
- **Signal Handling:** Handle Ctrl+C (SIGINT) and Ctrl+Z (SIGTSTP) signals gracefully.

### Code Overview
Here's a brief overview of the core functionalities in MyShell:

- **parseInput:** Parses user input to identify commands, arguments, and delimiters.
- **executeCommand:** Executes a single command with proper signal handling and error checking.
- **executeParallelCommands:** Runs multiple commands in parallel, waiting for all processes to finish.
- **executeSequentialCommands:** Executes multiple commands sequentially, ensuring proper order of execution.
- **executeCommandRedirection:** Redirects the output of a single command to a specified file.
- **executePipedCommands:** Handles command pipelines, allowing the output of one command to be used as input for another.
- **Signal Handlers:** Custom signal handlers for Ctrl+C (SIGINT) and Ctrl+Z (SIGTSTP) signals.

### How to Use
1. **Compilation:** Compile the source code using a C compiler (e.g., gcc).
2. **Execution:** Run the compiled executable to start MyShell.
3. **Command Entry:** Enter commands interactively, following the specified syntax for parallel, sequential execution, output redirection, and piping.
4. **Signal Handling:** MyShell gracefully handles Ctrl+C and Ctrl+Z signals without interrupting ongoing processes.

### Conclusion
MyShell offers a versatile and efficient environment for executing shell commands with additional functionalities for advanced users. With support for signal handling, parallel/sequential execution, output redirection, and piping, it provides a comprehensive solution for various command-line tasks.
