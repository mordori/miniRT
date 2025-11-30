/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_next_line.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 20:14:52 by myli-pen          #+#    #+#             */
/*   Updated: 2025/08/23 00:05:23 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "libft_io.h"
#include "libft_mem.h"
#include "libft_str.h"

static inline char	*join_lines(char *line, const char *buf);
static inline char	*extract_line(const char *buf);
static inline void	trimbuf(char *buf);
static inline size_t	linelen(const char *buf);

/**
 * Returns a line read from file descriptor `fd`.
 *
 * @param fd File descriptor to read from.
 * @param line Out line read.
 * @return Status code of the operation performed.
 */
int	get_next_line(int fd, char **line)
{
	static char	buf[BUFFER_SIZE + 1];
	ssize_t		bytes;

	if (fd < 0 || BUFFER_SIZE < 1 || !line)
		return (GNL_ERROR);
	*line = extract_line(buf);
	if (buf[0] && !*line)
		return (GNL_ERROR);
	while (!ft_strchr(buf, '\n'))
	{
		bytes = read(fd, buf, BUFFER_SIZE);
		if (bytes == ERROR)
			return (free(*line), GNL_ERROR);
		if (bytes == 0)
			break ;
		buf[bytes] = '\0';
		*line = join_lines(*line, buf);
		if (!*line)
			return (GNL_ERROR);
	}
	trimbuf(buf);
	if (*line)
		return (GNL_OK);
	return (GNL_EOF);
}

/**
 * Joins an extracted string from the `buf` to the line that has no newline yet.
 *
 * @param buf Buffer of read characters.
 * @return Extracted string with a newline.
 */
static inline char	*join_lines(char *line, const char *buf)
{
	char	*new_line;

	new_line = extract_line(buf);
	if (!new_line)
		return (NULL);
	return (ft_strjoin_free(line, new_line));
}

/**
 * Extracts a string from `buf`, NUL-terminating the result.
 *
 * @param buf Buffer of read characters.
 * @return Extracted string.
 */
static inline char	*extract_line(const char *buf)
{
	if (!buf[0])
		return (NULL);
	return (ft_substr(buf, 0, linelen(buf)));
}

/**
 * Removes a string with a newline from `buf`, then NUL-terminates the result.
 *
 * @param buf Buffer of read characters.
 */
static inline void	trimbuf(char *buf)
{
	size_t	len_nl;
	size_t	len_buf;

	len_nl = linelen(buf);
	len_buf = ft_strlen(buf);
	ft_memcpy(buf, buf + len_nl, len_buf);
	buf[len_buf - len_nl] = '\0';
}

/**
 * Counts the number of characters in a `buf` to an included newline,
 * or to NUL if not found.
 *
 * @param buf Buffer of read characters.
 * @return Number of characters in a buffer to an included newline,
 * or to NUL if not found.
 */
static inline size_t	linelen(const char *buf)
{
	size_t	len_nl;

	len_nl = 0;
	while (buf[len_nl] != '\n' && buf[len_nl])
		++len_nl;
	if (buf[len_nl] == '\n')
		++len_nl;
	return (len_nl);
}
