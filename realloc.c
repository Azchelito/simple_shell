#include "shell.h"

/**
 * custom_memset - fills memory with a specific byte value
 * @dest: pointer to the memory area
 * @value: the byte value to fill *dest with
 * @size: the number of bytes to be filled
 * Return: a pointer to the memory area dest
 */
char *custom_memset(char *dest, char value, unsigned int size)
{
	unsigned int i;

	for (i = 0; i < size; i++)
		dest[i] = value;
	return (dest);
}

/**
 * free_string_array - frees an array of strings
 * @str_array: array of strings to be freed
 */
void free_string_array(char **str_array)
{
	char **temp = str_array;

	if (!str_array)
		return;

	while (*str_array)
	{
		free(*str_array);
		str_array++;
	}

	free(temp);
}

/**
 * custom_realloc - reallocates a block of memory
 * @ptr: pointer to the previous memory block
 * @old_size: size of the previous memory block
 * @new_size: size of the new memory block
 * Return: pointer to the new memory block
 */
void *custom_realloc(void *ptr, unsigned int old_size, unsigned int new_size)
{
	char *new_ptr;

	if (!ptr)
		return (malloc(new_size));

	if (!new_size)
	{
		free(ptr);
		return (NULL);
	}

	if (new_size == old_size)
		return (ptr);

	new_ptr = malloc(new_size);
	if (!new_ptr)
		return (NULL);

	old_size = (old_size < new_size) ? old_size : new_size;

	while (old_size--)
		new_ptr[old_size] = ((char *)ptr)[old_size];

	free(ptr);
	return (new_ptr);
}
