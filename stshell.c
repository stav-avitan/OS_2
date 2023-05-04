#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdbool.h>

// when the user presses Ctrl+C, the function prints a newline character to the console.
void handle_signal(int sig)
{
    if (sig == SIGINT) {
        putchar('\n');
    }
}

void exitmsg()
{
    fputs("Exiting...\n", stdout);
    exit(EXIT_SUCCESS);
}

void errormsg()
{
    fputs("Oops! Something went wrong. Returning to the original shell...\n", stderr);
    exit(EXIT_FAILURE);
}

int main()
{
    int i;
    char *argv[10];
    char command[1024];
    char *token;

    //  Ctrl+C
    signal(SIGINT, handle_signal);

    while (1)
    {

        printf("stshell> ");

        fgets(command, 1024, stdin);
        command[strlen(command) - 1] = '\0'; // replace \n with \0

        i = 0;
        token = strtok(command, " ");
        while (token != NULL)
        {
            argv[i] = token;
            token = strtok(NULL, " ");
            i++;
        }
        argv[i] = NULL;

        if (argv[0] == NULL)
        {
            continue;
        }

        if (strcmp(command, "exit") == 0)
        {
            exitmsg();
        }

        pid_t pid = fork();

        if (pid == -1)
        {
            errormsg();
        }
        if (pid == 0) // child
        {
            //  Ctrl+C
            signal(SIGINT, SIG_DFL);

            int j = 0;
            while (argv[j] != NULL)
            {
                switch (get_operator_type(argv[j])) {
                    case REDIRECTION:
                        handle_redirection(argv[j], argv[j+1]);
                        argv[j] = NULL;
                        break;
                    case APPEND:
                        handle_append(argv[j], argv[j+1]);
                        argv[j] = NULL;
                        break;
                    case PIPE:
                        handle_pipe(argv, j);
                        j = -1;
                        break;
                }
                j++;
            }
            execvp(argv[0], argv); // execute the command
        }
        wait(NULL);
    }
    return 0;
}

enum OperatorType {
    REDIRECTION,
    APPEND,
    PIPE,
    NONE
};

int get_operator_type(char *arg) {
    if (strcmp(arg, ">") == 0) {
        return REDIRECTION;
    } else if (strcmp(arg, ">>") == 0) {
        return APPEND;
    } else if (strcmp(arg, "|") == 0) {
        return PIPE;
    } else {
        return NONE;
    }
}

void handle_redirection(char *operator, char *file) {
    int fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd < 0) {
        errormsg();
    }
    dup2(fd, STDOUT_FILENO);
    close(fd);
}

void handle_append(char *operator, char *file) {
    int fd = open(file, O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
    if (fd < 0) {
        errormsg();
    }
    dup2(fd, STDOUT_FILENO);
    close(fd);
}

void handle_pipe(char **argv, int j) {
    int pipes[2]; // 0 - read, 1 - write

    int pipecheck = pipe(pipes);
    if (pipecheck < 0)
    {
        errormsg();
    }

    pid_t pid2 = fork();
    if (pid2 == -1)
    {
        errormsg();
    }

    // Child process
    if (pid2 == 0)
    {

        close(pipes[0]);
        int pipesCount = 0;
        char *input[10];

        // Execute the first command
        while (strcmp(argv[pipesCount], "|"))
{
    input[pipesCount++] = argv[j++];
}
input[pipesCount] = NULL;

int duplicate = dup2(pipes[1], STDOUT_FILENO);
if (duplicate < 0)
{
    errormsg();
}
close(pipes[1]);

// Execute the first command in the pipe
execvp(input[0], input);


        wait(NULL);
    }
    return 0;
}