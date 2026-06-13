/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wshoweky <wshoweky@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 19:47:18 by wshoweky          #+#    #+#             */
/*   Updated: 2026/03/10 19:47:21 by wshoweky         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
#define PARSING_H

#include "defines.h"

typedef enum e_error {
	E_OK = 0,
	E_EMPTY,
	E_UNKNOWN_ID,
	E_ARGS,
	E_INVALID_NUM,
	E_RANGE,
	E_DUPLICATE,
	E_MALLOC,
	E_MISSING_OBJ,
	E_TEXTURE,
	E_MATERIAL,
	E_EMISSIVE,
	E_TOO_MANY,
	E_TOO_BIG,
	E_NO_CAMERA,
	E_INVALID_PAT,
	E_INVALID_COLOR,
} t_error;

typedef struct s_mat_entry {
	t_material material;
	bool defined;
} t_mat_entry;

#endif
