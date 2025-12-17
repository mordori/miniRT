/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 21:49:31 by myli-pen          #+#    #+#             */
/*   Updated: 2025/10/18 03:15:52 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "libft_str.h"

/**
 * Allocates memory and returns an array of strings by splitting string `s`
 * using the character `c` as the deliter.
 *
 * Last element is a NULL pointer.
 *
 * @param s Source string.
 * @param c Delimiter character.
 * @return New array of strings split from the string `s`.
 */
char	**ft_split(char const *s, char c)
{
	char	**strs;
	size_t	words;
	size_t	word_len;
	size_t	i;

	if (!s)
		return (NULL);
	words = ft_count_words(s, c);
	strs = malloc((words + 1) * sizeof(char *));
	if (!strs)
		return (NULL);
	i = 0;
	while (words--)
	{
		word_len = ft_word_len(&s, c);
		strs[i] = ft_substr(s - word_len, 0, word_len);
		if (!strs[i++])
		{
			ft_free_split(strs);
			return (NULL);
		}
	}
	strs[i] = NULL;
	return (strs);
}

/**
 * Counts the words inside string `s` delimited by the character `c`.
 *
 * @param s Source string.
 * @param c Delimiter character.
 * @return Number of words found.
 */
size_t	ft_count_words(char const *s, char c)
{
	size_t	count;

	count = 0;
	while (*s)
	{
		while (*s == c && *s)
			++s;
		while (*s != c && *s)
			++s;
		if (*s || *(s - 1) != c)
			++count;
	}
	return (count);
}

/**
 * Counts the length of a word in the source string pointed by `s` using
 * the delimiter character `c`.
 *
 * Moves the pointer to the end of the word.
 *
 * @param s Pointer to a source string.
 * @param c Delimiter character.
 * @return Length of a word.
 */
size_t	ft_word_len(char const **s, char c)
{
	size_t	len;

	len = 0;
	while (**s == c && **s)
		++*s;
	while (**s != c && **s)
	{
		++len;
		++*s;
	}
	return (len);
}

/**
 * Frees the allocated array of strings `s` from memory and
 * sets the pointers to NULL.
 *
 * @param s Array of strings.
 * @return NULL.
 */
void	ft_free_split(char **strs)
{
	int	i;

	i = 0;
	while (strs[i])
	{
		free(strs[i]);
		strs[i++] = NULL;
	}
	free(strs);
	strs = NULL;
}
