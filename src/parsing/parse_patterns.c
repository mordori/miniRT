#include "parsing.h"
#include "libft.h"

t_error parse_mat_pattern(t_material *mat, char **tkns, int tc)
{
	if  (tc < 13)
		return (E_OK);
	if (!parse_pattern_token(tkns[12], &mat->pattern))
		return (E_INVALID_NUM);
	if (mat->pattern == PAT_NONE)
		return (E_OK);
	if (tc < 14 || !parse_float(tkns[13], &mat->pattern_scale))
		return (E_INVALID_NUM);
	if (tc < 16 || !parse_color(tkns[14], &mat->albedo2))
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
	return (false);
}
