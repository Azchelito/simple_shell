#include "shell.h"

/**
 * _custom_strncpy - Copies a string up to a specified length.
 * @dest: Destination string.
 * @src: Source string.
 * @n: Number of characters to copy.
 * Return: Pointer to the destination string.
 */
char *_custom_strncpy(char *dest, const char *src, size_t n)
{
	size_t i;

	for (i = 0; i < n && src[i] != '\0'; i++)
		dest[i] = src[i];

	for (; i < n; i++)
		dest[i] = '\0';

	return (dest);
}

/**
 * _custom_strncat - Concatenates two strings up to a specified length.
 * @dest: Destination string.
 * @src: Source string.
 * @n: Maximum number of characters to concatenate.
 * Return: Pointer to the destination string.
 */
char *_custom_strncat(char *dest, const char *src, size_t n)
{
	size_t dest_len = _custom_strlen(dest);
	size_t i;

	for (i = 0; i < n && src[i] != '\0'; i++)
		dest[dest_len + i] = src[i];

	dest[dest_len + i] = '\0';

	return (dest);
}

/**
 * _custom_strchr - Locates a character in a string.
 * @s: String to be searched.
 * @c: Character to be located.
 * Return: Pointer to the first occurrence of character c in the string s,
 *         or NULL if the character is not found.
 */
char *_custom_strchr(const char *s, int c)
{
	while (*s != '\0')
	{
		if (*s == c)
			return (char *s);
		s++;
	}

	if (*s == c)
		return (char *s);

	return (NULl);
}
