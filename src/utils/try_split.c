#include <stdlib.h>

#include "libft_str.h"
#include "libft_utils.h"

static size_t count_words(const char* s) {
	size_t count = 0;
	while (s && *s) {
		while (*s && ft_isspace(*s))
			s++;
		while (*s && !ft_isspace(*s))
			s++;
		if (*(s - 1) && !ft_isspace(*(s - 1)))
			count++;
	}
	return count;
}

static size_t word_len(const char** s) {
	size_t len = 0;
	while (s && **s && ft_isspace(**s))
		(*s)++;
	while (s && (*s)[len] && !ft_isspace((*s)[len]))
		len++;
	*s += len;
	return len;
}

char** try_split(char const* s) {
	if (!s)
		return NULL;

	size_t words = count_words(s);
	char** strs = calloc((words + 1), sizeof(char*));
	if (!strs)
		return NULL;

	size_t i = 0;
	while (words--) {
		size_t w_len = word_len(&s);
		strs[i] = ft_substr(s - w_len, 0, w_len);
		if (!strs[i++]) {
			ft_free_split(strs);
			return NULL;
		}
	}
	strs[i] = NULL;
	return strs;
}
