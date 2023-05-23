# OperatingSystemProject

# Cwushell
Cwushell is a simple shell program implemented in C++. It provides a command-line interface for executing various commands and performing file operations. The shell supports basic functionalities like opening files, reading from files, writing to files, deleting files, retrieving file information, changing file permissions, executing external commands, and more.

## Prerequisites
To compile and run Cwushell, make sure you have the following installed:

- C++ compiler (e.g., g++)
- Standard C++ Library

## Getting Started
- Clone the repository or download the source code files.
- Navigate to the project directory.

### Compilation
Use the following command to compile the program:

`g++ main.cpp -o cwushell
`
### Execution

To run the shell, use the following command:

`./cwushell
`
## Usage
Once you run Cwushell, you can start entering commands at the prompt cwushell-> . The available commands are:

- open <filename>: Opens the specified file.
- read <filename>: Reads data from the specified file.
- write <filename> <data>: Writes data to the specified file.
- delete <filename>: Deletes the specified file.
- stat <filename>: Retrieves information about the specified file.
- chmod <filename> <mode>: Changes the permissions of the specified file.
- execute <command>: Executes an external command.
- getpid: Retrieves the process ID of the shell.
- getppid: Retrieves the parent process ID of the shell.
- getpriority <process_id>: Retrieves the priority of a process.
- nice <nice_value>: Sets the nice value of the shell.
- lseek <filename> <offset> <whence>: Seeks to a new position within the specified file.
- kill <process_id> <signal_number>: Sends a signal to a process.
You can exit the shell by entering the exit command.

## File Operations
Cwushell provides file-related commands such as open, read, write, delete, stat, chmod, and lseek. These commands allow you to perform operations on files, such as opening a file, reading its contents, writing data to it, deleting it, retrieving file information, changing file permissions, and seeking to a specific position within the file.

## External Command Execution
Cwushell supports executing external commands using the execute command. You can enter the name of the command you want to execute, and the shell will attempt to execute it. The shell uses execvp to execute the command, and the command's output will be displayed on the console.

## Process Information
Cwushell provides commands to retrieve process-related information. The getpid command retrieves the process ID of the shell itself, while the getppid command retrieves the parent process ID of the shell. The getpriority <process_id> command retrieves the priority of a process specified by its process ID.

## Nice Value
The nice <nice_value> command allows you to set the nice value of the shell. The nice value determines the priority of the process. Higher nice values indicate lower priority, while lower nice values indicate higher priority. The command returns the new nice value of the shell.

## Signal Handling
Cwushell includes the kill <process_id> <signal_number> command, which sends a signal to a specified process. By default, the command sends the SIGTERM signal. You can optionally specify a different signal number as the second argument.

## Command History
We have incorporated a basic command history functionality by making use of file I/O operations to store and to retrieve the command history.
  
## Acknowledgments
The implementation of cwushell was inspired by various shell programs and command line interfaces.
