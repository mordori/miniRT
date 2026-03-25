/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validator.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 22:10:21 by myli-pen          #+#    #+#             */
/*   Updated: 2026/03/25 22:11:03 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scene.h"
#include "utils.h"
#include "libft_str.h"

void	validate_file_type(char *file)
{
	const char		*dot;

	dot = ft_strchr(file, '.');
	if (!dot || ft_strcmp(dot, ".rt") != 0)
		fatal_error(NULL, "invalid file format", __FILE__, __LINE__);
}
