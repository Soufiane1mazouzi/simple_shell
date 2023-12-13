#include "shell.h"

/**
 * struct_info - initializes info_t struct
 * @info: struct address
 */
void struct_info(info_t *info)
{
	info->arg = NULL;
	info->argv = NULL;
	info->path = NULL;
	info->argc = 0;
}

/**
 * init_info - initializes info_t struct
 * @info: struct address
 * @av: argument vector
 */
void init_info(info_t *info, char **av)
{
	int i = 0;

	info->fname = av[0];
	if (info->arg)
	{
		info->argv = splitstr(info->arg, " \t");
		if (!info->argv)
		{

			info->argv = malloc(sizeof(char *) * 2);
			if (info->argv)
			{
				info->argv[0] = _dupstr(info->arg);
				info->argv[1] = NULL;
			}
		}
		for (i = 0; info->argv && info->argv[i]; i++)
			;
		info->argc = i;

		rename_alias(info);
		output_vars(info);
	}
}

/**
 * cancel_info - frees info_t struct fields
 * @info: struct address
 * @all: true if freeing all fields
 */
void cancel_info(info_t *info, int all)
{
	ffree(info->argv);
	info->argv = NULL;
	info->path = NULL;
	if (all)
	{
		if (!info->cmd_buf)
			free(info->arg);
		if (info->env)
			frees_all(&(info->env));
		if (info->history)
			frees_all(&(info->history));
		if (info->alias)
			frees_all(&(info->alias));
		ffree(info->environ);
			info->environ = NULL;
		freenull((void **)info->cmd_buf);
		if (info->readfd > 2)
			close(info->readfd);
		_putchar(BUF_FLUSH);
	}
}
