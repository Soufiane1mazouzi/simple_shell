#include "shell.h"

/**
 * hsh - main shell loop
 * @info: the parameter & return info struct
 * @av: the argument vector from main()
 *
 * Return: 0 on success, 1 on error, or error code
 */
int hsh(info_t *info, char **av)
{
	ssize_t r = 0;
	int builtin_ret = 0;

	while (r != -1 && builtin_ret != -2)
	{
		struct_info(info);
		if (activated(info))
			_puts("$ ");
		_printstderr(BUF_FLUSH);
		r = result_input(info);
		if (r != -1)
		{
			init_info(info, av);
			builtin_ret = cmd_find(info);
			if (builtin_ret == -1)
				path_find(info);
		}
		else if (activated(info))
			_putchar('\n');
		cancel_info(info, 0);
	}
	output_file(info);
	cancel_info(info, 1);
	if (!activated(info) && info->status)
		exit(info->status);
	if (builtin_ret == -2)
	{
		if (info->err_num == -1)
			exit(info->status);
		exit(info->err_num);
	}
	return (builtin_ret);
}

/**
 * cmd_find - finds a builtin command
 * @info: the parameter & return info struct
 *
 * Return: -1 if builtin not found,
 *			0 if builtin executed successfully,
 *			1 if builtin found but not successful,
 *			-2 if builtin signals exit()
 */
int cmd_find(info_t *info)
{
	int i, built_in_ret = -1;
	builtin_table builtintbl[] = {
		{"exit", _shellexit},
		{"env", _envarg},
		{"help", _mvcd},
		{"history", _historylist},
		{"setenv", _setenvarg},
		{"unsetenv", _unsetenvarg},
		{"cd", _cdchange},
		{"alias", _modifiedalias},
		{NULL, NULL}
	};

	for (i = 0; builtintbl[i].type; i++)
		if (_strcomparison(info->argv[0], builtintbl[i].type) == 0)
		{
			info->line_count++;
			built_in_ret = builtintbl[i].func(info);
			break;
		}
	return (built_in_ret);
}

/**
 * path_find - finds a command in PATH
 * @info: the parameter & return info struct
 *
 * Return: void
 */
void path_find(info_t *info)
{
	char *path = NULL;
	int i, k;

	info->path = info->argv[0];
	if (info->linecount_flag == 1)
	{
		info->line_count++;
		info->linecount_flag = 0;
	}
	for (i = 0, k = 0; info->arg[i]; i++)
		if (!is_delimeter(info->arg[i], " \t\n"))
			k++;
	if (!k)
		return;

	path = cmd_path(info, _getenvarg(info, "PATH="), info->argv[0]);
	if (path)
	{
		info->path = path;
		cmd_run(info);
	}
	else
	{
		if ((activated(info) || _getenvarg(info, "PATH=")
			|| info->argv[0][0] == '/') && exe_cmd(info, info->argv[0]))
			cmd_run(info);
		else if (*(info->arg) != '\n')
		{
			info->status = 127;
			output_error(info, "not found\n");
		}
	}
}

/**
 * cmd_run - forks a an exec thread to run cmd
 * @info: the parameter & return info struct
 *
 * Return: void
 */
void cmd_run(info_t *info)
{
	pid_t child_pid;

	child_pid = fork();
	if (child_pid == -1)
	{
		/* TODO: PUT ERROR FUNCTION */
		perror("Error:");
		return;
	}
	if (child_pid == 0)
	{
		if (execve(info->path, info->argv, get_strenv(info)) == -1)
		{
			cancel_info(info, 1);
			if (errno == EACCES)
				exit(126);
			exit(1);
		}
		/* TODO: PUT ERROR FUNCTION */
	}
	else
	{
		wait(&(info->status));
		if (WIFEXITED(info->status))
		{
			info->status = WEXITSTATUS(info->status);
			if (info->status == 126)
				output_error(info, "Permission denied\n");
		}
	}
}
