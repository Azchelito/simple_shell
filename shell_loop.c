#include "shell.h"

/**
 * custom_shell_loop - main shell loop
 * @shell_info: the parameter & return info struct
 * @arguments: the argument vector from main()
 *
 * Return: 0 on success, 1 on error, or error code
 */
int custom_shell_loop(info_t *shell_info, char **arguments)
{
	ssize_t read_status = 0;
	int builtin_result = 0;

	while (read_status != -1 && builtin_result != -2)
	{
		clear_shell_info(shell_info);
		if (is_interactive(shell_info))
			_puts("$ ");
		_eputchar(BUF_FLUSH);
		read_status = read_input(shell_info);
		if (read_status != -1)
		{
			set_shell_info(shell_info, arguments);
			builtin_result = find_custom_builtin(shell_info);
			if (builtin_result == -1)
				find_custom_command(shell_info);
		}
		else if (is_interactive(shell_info))
			_putchar('\n');
		free_shell_info(shell_info, 0);
	}
	write_shell_history(shell_info);
	free_shell_info(shell_info, 1);
	if (!is_interactive(shell_info) && shell_info->status)
		exit(shell_info->status);
	if (builtin_result == -2)
	{
		if (shell_info->error_number == -1)
			exit(shell_info->status);
		exit(shell_info->error_number);
	}
	return (builtin_result);
}

/**
 * find_custom_builtin - finds a custom builtin command
 * @shell_info: the parameter & return info struct
 *
 * Return: -1 if builtin not found,
 * 	0 if builtin executed successfully,
 * 	1 if builtin found but not successful,
 * 	2 if builtin signals exit()
 */
int find_custom_builtin(info_t *shell_info)
{
	int index, builtin_result = -1;
	builtin_table custom_builtins[] = {
		{"exit", _my_exit},
		{"env", _my_env},
		{"help", _my_help},
		{"history", _my_history},
		{"setenv", _my_setenv},
		{"unsetenv", _my_unsetenv},
		{"cd", _my_cd},
		{"alias", _my_alias},
		{NULL, NULL}
	};

	for (index = 0; custom_builtins[index].type; index++)
		if (_strcmp(shell_info->argv[0], custom_builtins[index].type) == 0)
		{
			shell_info->line_count++;
			builtin_result = custom_builtins[index].func(shell_info);
			break;
		}
	return (builtin_result);
}

/**
 * find_custom_command - finds a command in PATH
 * @shell_info: the parameter & return info struct
 *
 * Return: void
 */
void find_custom_command(info_t *shell_info)
{
	char *command_path = NULL;
	int i, count;

	shell_info->path = shell_info->argv[0];
	if (shell_info->line_count_flag == 1)
	{
		shell_info->line_count++;
		shell_info->line_count_flag = 0;
	}
	for (i = 0, count = 0; shell_info->arg[i]; i++)
		if (!is_delimiter(shell_info->arg[i], " \t\n"))
			count++;
	if (!count)
		return;

	command_path = find_path(shell_info, _getenv(shell_info, "PATH="), shell_info->argv[0]);
	if (command_path)
	{
		shell_info->path = command_path;
		fork_custom_command(shell_info);
	}
	else
	{
		if ((is_interactive(shell_info) || _getenv(shell_info, "PATH=")
					|| shell_info->argv[0][0] == '/') && is_valid_command(shell_info, shell_info->argv[0]))
			fork_custom_command(shell_info);
		else if (*(shell_info->arg) != '\n')
		{
			shell_info->status = 127;
			print_error(shell_info, "not found\n");
		}
	}
}

/**
 * fork_custom_command - forks an exec thread to run custom command
 * @shell_info: the parameter & return info struct
 *
 * Return: void
 */
void fork_custom_command(info_t *shell_info)
{
	pid_t child_pid;

	child_pid = fork();
	if (child_pid == -1)
	{
		perror("Error:");
		return;
	}
	if (child_pid == 0)
	{
		if (execve(shell_info->path, shell_info->argv, get_environ(shell_info)) == -1)
		{
			free_shell_info(shell_info, 1);
			if (errno == EACCES)
				exit(126);
			exit(1);
		}
	}
	else
	{
		wait(&(shell_info->status));
		if (WIFEXITED(shell_info->status))
		{
			shell_info->status = WEXITSTATUS(shell_info->status);
			if (shell_info->status == 126)
				print_error(shell_info, "Permission denied\n");
		}
	}
}
