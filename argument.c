#include <assert.h>
#include <string.h>
#include "argument.h"
#include "utility.h"
#include "message.h"

Operation get_tag_operation_type(const char *const arg_option) {

    assert(arg_option != NULL);

    if((strcmp(arg_option, ARG_MOD_TITLE) 		== 0)
	    || (strcmp(arg_option, ARG_MOD_TRACK) 	== 0)
	    || (strcmp(arg_option, ARG_MOD_ARTIST) 	== 0)
	    || (strcmp(arg_option, ARG_MOD_ALBUM) 	== 0)
	    || (strcmp(arg_option, ARG_MOD_YEAR) 	== 0)
	    || (strcmp(arg_option, ARG_MOD_COMMENT) 	== 0)
	    || (strcmp(arg_option, ARG_MOD_GENRE) 	== 0)) {
	return o_edit;
    }

    if(strcmp(arg_option, ARG_VERSION) == 0) {
	return o_read;
    }

    if(strcmp(arg_option, ARG_HELP) == 0) {
	return o_help;
    }

    return o_invalid;
}

Bool is_arg_option(const char *const arg_option) {

    assert(arg_option != NULL);

    if(get_tag_operation_type(arg_option) != o_invalid)
	return e_true;
    return e_false;
}

Status validate_args(char **argv, Tag_Manip_Info *tag_manip_info) {

    assert(argv != NULL && tag_manip_info != NULL);

    //check for args from the user.
    if(!argv[1]) {
	print_options_menu();
	return e_failure;
    }

    //check if the 1st arg is a valid option.
    if(!is_arg_option(argv[1])) {
	print_error_message(STR_ERR_INVALID_ARG);
	print_options_menu();
	return e_failure;
    }

    //store the arg and the operation type
    tag_manip_info->oper_option = argv[1];
    tag_manip_info->operation = get_tag_operation_type(argv[1]);

    /*
     * If the operation is read,
     * - then check for arg 2.
     * - check if arg 2 is an mp3 file.
     * - then store that filename.
     */
    if(tag_manip_info->operation == o_read) {
	if(!argv[2]) {
	    print_error_message(STR_ERR_NO_FILE_INPUT);
	    print_usage_format();
	    return e_failure;
	}

	if(!is_file_mp3(argv[2])) {
	    fprintf(stderr, "File %s is not an MP3 file.\n", argv[2]);
	    return e_failure;
	}

	tag_manip_info->mp3_filename = argv[2];
    }

    /*
     * If the operation is edit,
     * - then check for arg 2.
     * - check if arg 2 is an edit value string.
     * - check for arg 3.
     * - check if arg 3 is an mp3 file.
     * - then store that filename.
     */
    if(tag_manip_info->operation == o_edit) {
	if(!argv[2]) {
	    print_error_message(STR_ERR_NO_VALUE_STR);
	    print_usage_format();
	    return e_failure;
	}

	if(!argv[3]) {
	    print_error_message(STR_ERR_NO_FILE_INPUT);
	    print_usage_format();
	    return e_failure;
	}

	if(!is_file_mp3(argv[3])) {
	    fprintf(stderr, "File %s is not an MP3 file.\n", argv[3]);
	    return e_failure;
	}

	tag_manip_info->mp3_filename = argv[3];
    }

    return e_success;
}
