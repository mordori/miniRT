#include "parsing.h"
#include "libft.h"

t_error parse_mat_pattern(t_material *mat, char **tkns, int tc)
{
	if  (tc < 16)
		return (E_MISSING_ARGS);
	if (!parse_pattern_token(tkns[13], &mat->pattern))
		return (E_INVALID_NUM);
	if (mat->pattern == PAT_NONE)
		return (E_OK);
	if (!parse_float(tkns[14], &mat->pattern_scale))
		return (E_INVALID_NUM);
	if (!parse_color(tkns[15], &mat->albedo2))
		return (E_INVALID_NUM);
	mat->base_color = BASE_PATTERN;
	return (E_OK);
}

bool	parse_pattern_token(const char *tkn, t_pattern *out)
{
	if (ft_strcmp(tkn, "none") == 0 || is_placeholder(tkn))
		return (*out = PAT_NONE, true);
	if (ft_strcmp(tkn, "checker") == 0)
		return (*out = PAT_CHECKERBOARD, true);
	if (ft_strcmp(tkn, "gradient") == 0)
		return (*out = PAT_GRADIENT, true);
	if (ft_strcmp(tkn, "stripes") == 0)
		return (*out = PAT_STRIPES, true);
	return (false);
}
