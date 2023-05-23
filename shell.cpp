#include <iostream>
#include <string.h>
#include <unistd.h>
#include <cstdlib>
#include <stdio.h>
#include <sys/wait.h>
#include <fcntl.h> //for open, read, write, etc
#include <fstream>
#include <sys/stat.h>   // for stat
#include <sys/types.h>  // for chmod, lseek
#include <cstdlib>      // for strtol
#include <signal.h>
#include <csignal>
#include <sys/resource.h>
#define TOKENSIZE 100

using namespace std;

void StrTokenizer(char* line, char** argv);
void myExecvp(char** argv);
int openFile(const char* filename, int flags);
ssize_t readFile(const char* filename, void* buf, size_t count);
ssize_t writeFile(const char* filename, const void* buf, size_t count);

int deleteFile(const char* filename);
int statFile(const char* filename, struct stat* fileInfo);
int changePermissions(const char* filename, mode_t mode);
off_t seekFile(int fd, off_t offset, int whence);
void handleKillCommand(char** argv);
int main()
{


    char* path2;
    char* arr[250];
    char* Tokenized;
    char input[250];
    char* argv[TOKENSIZE];

    ofstream historyFile("command_history.txt", ios::app); // Create/open the history file in append mode

    if (!historyFile.is_open()) {
        cout << "Failed to open history file" << endl;
        return 1;
    }

    while (true)
    {
        cout << "cwushell-> ";
        cin.getline(input, 250);

        // Store the command in the history file
        historyFile << input << endl;

        StrTokenizer(input, argv);
        if (strcmp(argv[0], "exit") == 0)
        {
            break;
        }
        else if (strcmp(input, "\n") == 0)
        {
            continue;
        }
        else if (strcmp(argv[0], "open") == 0)
        {
            if (argv[1] == NULL)
            {
                cout << "ERROR: Missing filename argument for open command" << endl;
                continue;
            }
            int flags = O_RDWR | O_CREAT; // Example flags for opening a file
            int fd = openFile(argv[1], flags);
            if (fd == -1)
            {
                cout << "ERROR: Failed to open file" << endl;
            }
            else
            {
                cout << "File opened successfully" << endl;
            }
        }
        else if (strcmp(argv[0], "read") == 0)
        {
            if (argv[1] == NULL)
            {
                cout << "ERROR: Missing filename argument for read command" << endl;
                continue;
            }
            char buffer[100]; // Example buffer for reading from file
            ssize_t bytesRead = readFile(argv[1], buffer, sizeof(buffer));
            if (bytesRead == -1)
            {
                cout << "ERROR: Failed to read from file" << endl;
            }
            else
            {
                cout << "Read " << bytesRead << " bytes: " << buffer << endl;
            }
        }
        else if (strcmp(argv[0], "write") == 0)
        {
            if (argv[1] == NULL)
            {
                cout << "ERROR: Missing filename argument for write command" << endl;
                continue;
            }
            if (argv[2] == NULL)
            {
                cout << "ERROR: Missing data argument for write command" << endl;
                continue;
            }
            ssize_t bytesWritten = writeFile(argv[1], argv[2], strlen(argv[2]));
            if (bytesWritten == -1)
            {
                cout << "ERROR: Failed to write to file" << endl;
            }
            else
            {
                cout << "Written " << bytesWritten << " bytes to file" << endl;
            }
        }
        
        else if (strcmp(argv[0], "delete") == 0)
        {
            if (argv[1] == NULL)
            {
                cout << "ERROR: Missing filename argument for delete command" << endl;
                continue;
            }
            int result = deleteFile(argv[1]);
            if (result == -1)
            {
                cout << "ERROR: Failed to delete file" << endl;
            }
            else
            {
                cout << "File deleted successfully" << endl;
            }
        }
        else if (strcmp(argv[0], "stat") == 0)
        {
            if (argv[1] == NULL)
            {
                cout << "ERROR: Missing filename argument for stat command" << endl;
                continue;
            }
            struct stat fileInfo;
            int result = statFile(argv[1], &fileInfo);
            if (result == -1)
            {
                cout << "ERROR: Failed to retrieve file information" << endl;
            }
            else
            {
                // Process the file information as needed
                // Example: Display file size and permissions
                cout << "File Size: " << fileInfo.st_size << " bytes" << endl;
                cout << "File Permissions: " << (fileInfo.st_mode & S_IRWXU) << endl;
            }
        }
        else if (strcmp(argv[0], "chmod") == 0)
        {
            if (argv[1] == NULL)
            {
                cout << "ERROR: Missing filename argument for chmod command" << endl;
                continue;
            }
            if (argv[2] == NULL)
            {
                cout << "ERROR: Missing mode argument for chmod command" << endl;
                continue;
            }
            mode_t mode = strtol(argv[2], NULL, 8);  // Convert mode from string to octal
            int result = changePermissions(argv[1], mode);
            if (result == -1)
            {
                cout << "ERROR: Failed to change file permissions" << endl;
            }
            else
            {
                cout << "File permissions changed successfully" << endl;
            }
        }
        
      else if (strcmp(argv[0], "execute") == 0)
{
    if (argv[1] == NULL)
    {
        cout << "ERROR: Missing command argument for execute command" << endl;
        continue;
    }

    pid_t pid = fork();
    if (pid == -1)
    {
        cout << "ERROR: Failed to create child process" << endl;
        continue;
    }
    else if (pid == 0)
    {
        // Child process
        execvp(argv[1], &argv[1]);
        
        exit(1);
    }
    else
    {
        // Parent process
        // Wait for the child process to finish
        int status;
        pid_t finishedChild = wait(&status);
        if (finishedChild == -1)
        {
            cout << "ERROR: Failed to wait for child process" << endl;
        }
        else
        {
            cout << "Child process " << finishedChild << " has finished with status " << status << endl;
        }
    }
}

else if (strcmp(argv[0], "getpid") == 0) {
    pid_t pid = getpid();
    cout << "Process ID: " << pid << endl;
}

else if (strcmp(argv[0], "getppid") == 0) {
    pid_t parentPid = getppid();
    cout << "Parent Process ID: " << parentPid << endl;
}

else if (strcmp(argv[0], "getpriority") == 0) {
            if (argv[1] == nullptr) {
                cout << "ERROR: Missing process ID argument for getpriority command" << endl;
                continue;
            }
            
            int processId = atoi(argv[1]);
            int priority = getpriority(PRIO_PROCESS, processId);
            
            if (priority == -1) {
                cout << "ERROR: Failed to get priority for process ID: " << processId << endl;
            } else {
                cout << "Priority for process ID " << processId << ": " << priority << endl;
            }
        }
        
        else if (strcmp(argv[0], "nice") == 0) {
            if (argv[1] == nullptr) {
                cout << "ERROR: Missing nice value argument for nice command" << endl;
                continue;
            }
            
            int niceValue = atoi(argv[1]);
            int result = nice(niceValue);
            
            if (result == -1) {
                cout << "ERROR: Failed to set nice value" << endl;
            } else {
                cout << "Nice value set to: " << result << endl;
            }
        }
        
        else if (strcmp(argv[0], "lseek") == 0)
        {
            if (argv[1] == NULL)
            {
                cout << "ERROR: Missing filename argument for lseek command" << endl;
                continue;
            }
            if (argv[2] == NULL)
            {
                cout << "ERROR: Missing offset argument for lseek command" << endl;
                continue;
            }
            if (argv[3] == NULL)
            {
                cout << "ERROR: Missing whence argument for lseek command" << endl;
                continue;
            }
            int fd = openFile(argv[1], O_RDONLY);  // Open file for reading
            if (fd == -1)
            {
                cout << "ERROR: Failed to open file" << endl;
                continue;
            }
            off_t offset = strtol(argv[2], NULL, 10);  // Convert offset from string to long
            int whence = strtol(argv[3], NULL, 10);    // Convert whence from string to long
            off_t newPosition = seekFile(fd, offset, whence);
            if (newPosition == -1)
            {
                cout << "ERROR: Failed to seek file" << endl;
            }
            else
            {
                cout << "New file position: " << newPosition << endl;
            }
            close(fd); // Closed file using file descriptor instead of filename
        }
        else
        {
            myExecvp(argv);
        }
    }

    historyFile.close(); // Close the history file
    
    return 0;
}

    
void myExecvp(char** argv)
{
    pid_t pid;
    int status;
    int childStatus;
    pid = fork();
    if (pid == 0)
    {
        childStatus = execvp(argv[0], argv); // Replaced *argv with argv[0]
        if (childStatus < 0)
        {
            cout << "ERROR: wrong input" << endl;
            exit(1); // Exit with non-zero status code on error
        }
        exit(0);
    }
    else if (pid < 0)
    {
        cout << "ERROR: fork failed" << endl;
    }
    else
    {
        wait(&childStatus); // Wait for the child process to finish
        
        if (WIFEXITED(childStatus))
        {
            // Child process terminated normally
            int exitStatus = WEXITSTATUS(childStatus);
            std::cout << "Child process exited with status: " << exitStatus << std::endl;
        }
        else if (WIFSIGNALED(childStatus))
        {
            // Child process terminated due to a signal
            int signalNumber = WTERMSIG(childStatus);
            std::cout << "Child process terminated by signal: " << signalNumber << std::endl;
        }
    }
}
    

void StrTokenizer(char* input, char** argv)
{
    char* stringTokenized;
    stringTokenized = strtok(input, " ");
    int i = 0;
    while (stringTokenized != NULL && i < TOKENSIZE - 1) // Added check to avoid exceeding array bounds
    {
        argv[i++] = stringTokenized;
        stringTokenized = strtok(NULL, " ");
    }
    argv[i] = NULL;
}

int openFile(const char* filename, int flags)
{
    int fd = open(filename, flags);
    return fd;
}

ssize_t readFile(const char* filename, void* buf, size_t count)
{
    int fd = open(filename, O_RDONLY);
    if (fd == -1)
    {
        return -1;
    }
    ssize_t bytesRead = read(fd, buf, count);
    close(fd);
    return bytesRead;
}

ssize_t writeFile(const char* filename, const void* buf, size_t count)
{
    int fd = open(filename, O_WRONLY | O_CREAT, 0644);
    if (fd == -1)
    {
        return -1;
    }
    ssize_t bytesWritten = write(fd, buf, count);
    close(fd);
    return bytesWritten;
}




int deleteFile(const char* filename)
{
    int result = unlink(filename);
    return result;
}

int statFile(const char* filename, struct stat* fileInfo)
{
    int result = stat(filename, fileInfo);
    return result;
}

int changePermissions(const char* filename, mode_t mode)
{
    int result = chmod(filename, mode);
    return result;
}

off_t seekFile(int fd, off_t offset, int whence)
{
    off_t newPosition = lseek(fd, offset, whence);
    return newPosition;
}

void handleKillCommand(char** argv)
{
    if (argv[1] == nullptr)
    {
        cout << "ERROR: Missing process ID argument for kill command" << endl;
        return;
    }

    int processID = atoi(argv[1]);
    int signalNumber = SIGTERM; // Default signal number is SIGTERM

    if (argv[2] != nullptr)
    {
        signalNumber = atoi(argv[2]);
    }

    int killResult = kill(processID, signalNumber);
    if (killResult == 0)
    {
        cout << "Process with ID " << processID << " killed successfully" << endl;
    }
    else
    {
        cout << "Failed to kill process with ID " << processID << endl;
    }
}
