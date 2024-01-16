#ifndef ARGUMENTS_H
#define ARGUMENTS_H

#include "types.h"

#define ARG_HELP 	"-h"
#define ARG_VERSION 	"-v"
#define ARG_MOD_TITLE 	"-t"
#define ARG_MOD_TRACK 	"-T"
#define ARG_MOD_ARTIST 	"-a"
#define ARG_MOD_ALBUM 	"-A"
#define ARG_MOD_YEAR 	"-y"
#define ARG_MOD_COMMENT "-c"
#define ARG_MOD_GENRE 	"-g"

Operation get_tag_operation_type(const char *const arg_option);

Bool is_arg_option(const char *const arg_option);

Status validate_args(char **argv, Tag_Manip_Info *tag_manip_info);

#endif
