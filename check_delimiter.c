#include "shell.h"

/**
 * check_delimiter - test if current char in buffer is a delimiter
 * @info: the parameter struct
 * @buf: the char buffer
 * @p: address of current position in buf
 *
 * Return: 1 if delimiter, 0 otherwise
 */
int check_delimiter(info_t *info, char *buf, size_t *p)
{
    size_t j = *p;

    if (buf[j] == '|' && buf[j + 1] == '|')
    {
        buf[j] = 0;
        j++;
        info->cmd_buf_type = CMD_OR;
    }
    else if (buf[j] == '&' && buf[j + 1] == '&')
    {
        buf[j] = 0;
        j++;
        info->cmd_buf_type = CMD_AND;
    }
    else if (buf[j] == ';') /* found end of this command */
    {
        buf[j] = 0; /* replace semicolon with null */
        info->cmd_buf_type = CMD_CHAIN;
    }
    else
        return (0);
    *p = j;
    return (1);
}

/**
 * evaluate_chain - evaluates whether to continue chaining based on last status
 * @info: the parameter struct
 * @buf: the char buffer
 * @p: address of current position in buf
 * @i: starting position in buf
 * @len: length of buf
 *
 * Return: Void
 */
void evaluate_chain(info_t *info, char *buf, size_t *p, size_t i, size_t len)
{
    size_t j = *p;

    if (info->cmd_buf_type == CMD_AND)
    {
        if (info->status)
        {
            buf[i] = 0;
            j = len;
        }
    }
    if (info->cmd_buf_type == CMD_OR)
    {
        if (!info->status)
        {
            buf[i] = 0;
            j = len;
        }
    }

    *p = j;
}

/**
 * update_aliases - updates aliases in the tokenized string
 * @info: the parameter struct
 *
 * Return: 1 if replaced, 0 otherwise
 */
int update_aliases(info_t *info)
{
    int i;
    list_t *node;
    char *p;

    for (i = 0; i < 10; i++)
    {
        node = node_starts_with(info->alias, info->arguments[0], '=');
        if (!node)
            return (0);
        free(info->arguments[0]);
        p = _strchr(node->str, '=');
        if (!p)
            return (0);
        p = _strdup(p + 1);
        if (!p)
            return (0);
        info->arguments[0] = p;
    }
    return (1);
}

/**
 * update_variables - updates variables in the tokenized string
 * @info: the parameter struct
 *
 * Return: 1 if replaced, 0 otherwise
 */
int update_variables(info_t *info)
{
    int i = 0;
    list_t *node;

    for (i = 0; info->arguments[i]; i++)
    {
        if (info->arguments[i][0] != '$' || !info->arguments[i][1])
            continue;

        if (!_strcmp(info->arguments[i], "$?"))
        {
            replace_string(&(info->arguments[i]),
                            _strdup(convert_number(info->status, 10, 0)));
            continue;
        }
        if (!_strcmp(info->arguments[i], "$$"))
        {
            replace_string(&(info->arguments[i]),
                            _strdup(convert_number(getpid(), 10, 0)));
            continue;
        }
        node = node_starts_with(info->env, &info->arguments[i][1], '=');
        if (node)
        {
            replace_string(&(info->arguments[i]),
                            _strdup(_strchr(node->str, '=') + 1));
            continue;
        }
        replace_string(&info->arguments[i], _strdup(""));
    }
    return (0);
}

/**
 * modify_string - replaces string
 * @old: address of old string
 * @new: new string
 *
 * Return: 1 if replaced, 0 otherwise
 */
int modify_string(char **old, char *new)
{
    free(*old);
    *old = new;
    return (1);
}
