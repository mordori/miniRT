#ifndef PARSING_H
# define PARSING_H

#include "defines.h"

typedef struct s_parser
{
    int line; // current line number
    bool has_light;
    bool has_ambient;
    bool has_camera;
} t_parser;



#endif