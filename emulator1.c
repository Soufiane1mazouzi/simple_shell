#include "shell.h"

/**
 * exit_shell - exits the shell
 * @info: Structure containing potential arguments.
 * Return: exits with a given exit status
 *         (0) if info->arguments[0] != "exit"
 */
int exit_shell(info_t *info)
{
    int exit_check;

    if (info->arguments[1]) /* If there is an exit argument */
    {
        exit_check = _custom_atoi(info->arguments[1]);
        if (exit_check == -1)
        {
            info->status = 2;
            print_custom_error(info, "Illegal number: ");
            _eputs(info->arguments[1]);
            _eputchar('\n');
            return (1);
        }
        info->err_num = _custom_atoi(info->arguments[1]);
        return (-2);
    }
    info->err_num = -1;
    return (-2);
}

/**
 * change_directory - changes the current directory of the process
 * @info: Structure containing potential arguments.
 * Return: Always 0
 */
int change_directory(info_t *info)
{
    char *s, *dir, buffer[1024];
    int chdir_ret;

    s = getcwd(buffer, 1024);
    if (!s)
        _puts("TODO: >>getcwd failure emsg here<<\n");
    if (!info->arguments[1])
    {
        dir = _getenv(info, "HOME=");
        if (!dir)
            chdir_ret = chdir((dir = _getenv(info, "PWD=")) ? dir : "/");
        else
            chdir_ret = chdir(dir);
    }
    else if (_strcmp(info->arguments[1], "-") == 0)
    {
        if (!_getenv(info, "OLDPWD="))
        {
            _puts(s);
            _putchar('\n');
            return (1);
        }
        _puts(_getenv(info, "OLDPWD=")), _putchar('\n');
        chdir_ret = chdir((dir = _getenv(info, "OLDPWD=")) ? dir : "/");
    }
    else
        chdir_ret = chdir(info->arguments[1]);
    if (chdir_ret == -1)
    {
        print_custom_error(info, "can't cd to ");
        _eputs(info->arguments[1]), _eputchar('\n');
    }
    else
    {
        _setenv(info, "OLDPWD", _getenv(info, "PWD="));
        _setenv(info, "PWD", getcwd(buffer, 1024));
    }
    return (0);
}

/**
 * display_help - displays help information
 * @info: Structure containing potential arguments.
 * Return: Always 0
 */
int display_help(info_t *info)
{
    char **arg_array;

    arg_array = info->arguments;
    _puts("help call works. Function not yet implemented \n");
    if (0)
        _puts(*arg_array); /* temp att_unused workaround */
    return (0);
}
