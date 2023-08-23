#include "shell.h"

/**
 * get_environment - returns a copy of the environment variables
 * @info: Structure containing relevant information
 * Return: Copy of the environment variables
 */
char **get_environment(info_t *info)
{
	if (!info->environment || info->env_changed)
	{
		info->environment = list_to_strings(info->env);
		info->env_changed = 0;
	}

	return (info->environment);
}

/**
 * unset_environment_variable - Remove an environment variable
 * @info: Structure containing relevant information
 * @variable: Name of the environment variable to unset
 * Return: 1 if variable is deleted, 0 otherwise
 */
int unset_environment_variable(info_t *info, char *variable)
{
	list_t *node = info->env;
	size_t index = 0;
	char *p;

	if (!node || !variable)
		return (0);

	while (node)
	{
		p = starts_with(node->str, variable);
		if (p && *p == '=')
		{
			info->env_changed = delete_node_at_index(&(info->env), index);
			index = 0;
			node = info->env;
			continue;
		}
		node = node->next;
		index++;
	}
	return (info->env_changed);
}

/**
 * set_environment_variable - Initialize a new environment variable
 *                            or modify an existing one
 * @info: Structure containing relevant information
 * @variable: Name of the environment variable
 * @value: Value to be assigned to the environment variable
 * Return: Always 0
 */
int set_environment_variable(info_t *info, char *variable, char *value)
{
	char *buf = NULL;
	list_t *node;
	char *p;

	if (!variable || !value)
		return (0);

	buf = malloc(_strlen(variable) + _strlen(value) + 2);
	if (!buf)
		return (1);
	_custom_strcpy(buf, variable);
	_custom_strcat(buf, "=");
	_custom_strcat(buf, value);
	node = info->env;
	while (node)
	{
		p = starts_with(node->str, variable);
		if (p && *p == '=')
		{
			free(node->str);
			node->str = buf;
			info->env_changed = 1;
			return (0);
		}
		node = node->next;
	}
	add_node_end(&(info->env), buf, 0);
	free(buf);
	info->env_changed = 1;
	return (0);
}
