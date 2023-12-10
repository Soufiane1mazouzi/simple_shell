#include "shell.h"

/**
 * new_env - prints the current environment
 * @info: Structure containing potential arguments.
 * Return: Always 0
 */
int new_env(info_t *info)
{
    print_list_str(info->env);
    return (0);
}

/**
 * get_new_env - gets the value of an environment variable
 * @info: Structure containing potential arguments.
 * @name: environment variable name
 *
 * Return: the value
 */
char *get_new_env(info_t *info, const char *name)
{
    list_t *node = info->env;
    char *p;

    while (node)
    {
        p = starts_with(node->str, name);
        if (p && *p)
            return (p);
        node = node->next;
    }
    return (NULL);
}

/**
 * new_set_env - Initialize a new environment variable,
 *             or modify an existing one
 * @info: Structure containing potential arguments.
 * Return: Always 0
 */
int new_set_env(info_t *info)
{
    if (info->argc != 3)
    {
        _eputs("Incorrect number of arguments\n");
        return (1);
    }
    if (set_new_env(info, info->argv[1], info->argv[2]))
        return (0);
    return (1);
}

/**
 * new_unset_env - Remove an environment variable
 * @info: Structure containing potential arguments.
 * Return: Always 0
 */
int new_unset_env(info_t *info)
{
    int i;

    if (info->argc == 1)
    {
        _eputs("Too few arguments.\n");
        return (1);
    }
    for (i = 1; i <= info->argc; i++)
        unset_new_env(info, info->argv[i]);

    return (0);
}

/**
 * populate_new_env_list - populates environment linked list
 * @info: Structure containing potential arguments.
 * Return: Always 0
 */
int populate_new_env_list(info_t *info)
{
    list_t *node = NULL;
    size_t i;

    for (i = 0; environ[i]; i++)
        add_node_end(&node, environ[i], 0);
    info->env = node;
    return (0);
}
