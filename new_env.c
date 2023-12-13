#include "shell.h"

/**
 * _envarg - prints the current environment
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * Return: Always 0
 */
int _envarg(info_t *info)
{
	output_str_linked(info->env);
	return (0);
}

/**
 * _getenvarg - gets the value of an environ variable
 * @info: Structure containing potential arguments. Used to maintain
 * @name: env var name
 *
 * Return: the value
 */
char *_getenvarg(info_t *info, const char *name)
{
	list_t *node = info->env;
	char *p;

	while (node)
	{
		p = needle_starts(node->str, name);
		if (p && *p)
			return (p);
		node = node->next;
	}
	return (NULL);
}

/**
 * _setenvarg - Initialize a new environment variable,
 *             or modify an existing one
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: Always 0
 */
int _setenvarg(info_t *info)
{
	if (info->argc != 3)
	{
		_inputstring("Incorrect number of arguements\n");
		return (1);
	}
	if (_setstrenv(info, info->argv[1], info->argv[2]))
		return (0);
	return (1);
}

/**
 * _unsetenvarg - Remove an environment variable
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: Always 0
 */
int _unsetenvarg(info_t *info)
{
	int i;

	if (info->argc == 1)
	{
		_inputstring("Too few arguements.\n");
		return (1);
	}
	for (i = 1; i <= info->argc; i++)
		_unsetatrenv(info, info->argv[i]);

	return (0);
}

/**
 * populate_envarg_list - populates env linked list
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * Return: Always 0
 */
int populate_envarg_list(info_t *info)
{
	list_t *node = NULL;
	size_t i;

	for (i = 0; environ[i]; i++)
		end_list_node(&node, environ[i], 0);
	info->env = node;
	return (0);
}
