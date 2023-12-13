#include <stdio.h>
#include <stdlib.h>

/**
 * main - Simple shell program
 *
 * Return: Always 0.
 */
int main(void)
{
    char *buffer = NULL;
    size_t bufsize = 0;

    while (1)
    {
        printf("($) ");

        if (getline(&buffer, &bufsize, stdin) == -1)
        {
            perror("getline");
            exit(EXIT_FAILURE);
        }

        /* Remove newline character */
        if (buffer[bufsize - 1] == '\n')
            buffer[bufsize - 1] = '\0';

        if (strcmp(buffer, "exit") == 0)
        {
            free(buffer);
            exit(EXIT_SUCCESS);
        }

        /* Implement command execution logic here */

        free(buffer);
        buffer = NULL;
        bufsize = 0;
    }

    return (0);
}
