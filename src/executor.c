#include "executor.h"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int execute_command(Command *command)
{
    if (command == NULL || command->argc == 0)
        return 0;

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return -1;
    }

    if (pid == 0) {

        if (command->input_file != NULL) {
            int fd = open(command->input_file, O_RDONLY);

            if (fd < 0) {
                perror(command->input_file);
                _exit(EXIT_FAILURE);
            }

            if (dup2(fd, STDIN_FILENO) < 0) {
                perror("dup2");
                close(fd);
                _exit(EXIT_FAILURE);
            }

            close(fd);
        }

        if (command->output_file != NULL) {
            int flags = O_WRONLY | O_CREAT;

            if (command->append)
                flags |= O_APPEND;
            else
                flags |= O_TRUNC;

            int fd = open(command->output_file, flags, 0644);

            if (fd < 0) {
                perror(command->output_file);
                _exit(EXIT_FAILURE);
            }

            if (dup2(fd, STDOUT_FILENO) < 0) {
                perror("dup2");
                close(fd);
                _exit(EXIT_FAILURE);
            }

            close(fd);
        }

        execvp(command->argv[0], command->argv);

        perror(command->argv[0]);
        _exit(EXIT_FAILURE);
    }

    if (command->background)
        return 0;

    int status;

    if (waitpid(pid, &status, 0) < 0) {
        perror("waitpid");
        return -1;
    }

    if (WIFEXITED(status))
        return WEXITSTATUS(status);

    return -1;
}

int execute_pipeline(Command *commands, int command_count)
{
    if (commands == NULL || command_count < 2)
        return -1;

    int pipes[command_count - 1][2];
    pid_t pids[command_count];

    for (int i = 0; i < command_count - 1; i++) {

        if (pipe(pipes[i]) < 0) {
            perror("pipe");
            return -1;
        }
    }

    for (int i = 0; i < command_count; i++) {

        pids[i] = fork();

        if (pids[i] < 0) {
            perror("fork");
            return -1;
        }

        if (pids[i] == 0) {

            if (i > 0) {
                if (dup2(pipes[i - 1][0], STDIN_FILENO) < 0) {
                    perror("dup2");
                    _exit(EXIT_FAILURE);
                }
            }

            if (i < command_count - 1) {
                if (dup2(pipes[i][1], STDOUT_FILENO) < 0) {
                    perror("dup2");
                    _exit(EXIT_FAILURE);
                }
            }

            for (int j = 0; j < command_count - 1; j++) {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }

            execvp(commands[i].argv[0], commands[i].argv);

            perror(commands[i].argv[0]);
            _exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < command_count - 1; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }

    for (int i = 0; i < command_count; i++)
        waitpid(pids[i], NULL, 0);

    return 0;
}
