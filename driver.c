#include "argument.h"
#include "utility.h"
#include "message.h"
#include "id3.h"
#include "id3v2.h"
#include <string.h>

int main(int argc, char **argv) {

    Tag_Manip_Info tag_manip_info;

    //making object well defined.
    tag_manip_info.fptr_mp3_file = NULL;
    tag_manip_info.mp3_filename = NULL;
    tag_manip_info.operation = o_invalid;
    tag_manip_info.oper_option = NULL;
    tag_manip_info.tag_version = ID3_invalid;
    tag_manip_info.tag_sub_version = ID3_sv_invalid;
    tag_manip_info.id3v2_tag = NULL;
    tag_manip_info.file_content = NULL;

    //vaidate user args.
    if(validate_args(argv, &tag_manip_info) == e_failure) {
	print_error_message(STR_ERR_ARG_VAL);
	return 1;
    }

    //display help menu if requested.
    if(tag_manip_info.operation == o_help) {
	print_options_menu();
	return 0;
    }

    //open mp3 file.
    if(open_file(&tag_manip_info, "rb") == e_failure) {
	print_error_message(STR_ERR_FILEIO_FAIL);
	return 1;
    }

    //validate id3 tag in the mp3 file.
    if(validate_ID3_tag(&tag_manip_info) == e_failure) {
	print_error_message(STR_ERR_TAG_VAL_FAIL);
	fclose(tag_manip_info.fptr_mp3_file);
	return 1;
    }

    //validate id3 sub-version in the mp3 file.
    if(validate_sub_version(&tag_manip_info) == e_failure) {
	print_error_message(STR_ERR_TAG_VAL_FAIL);
	fclose(tag_manip_info.fptr_mp3_file);
	return 1;
    }

    //allocate memory for the id3 tag.
    if(alloc_mem_for_tag(&tag_manip_info) == e_failure) {
	fclose(tag_manip_info.fptr_mp3_file);
	return 1;
    }

    //read id3 tag.
    if(read_id3_tag(&tag_manip_info) == e_failure) {
	dealloc_tag(&tag_manip_info);
	fclose(tag_manip_info.fptr_mp3_file);
	return 1;
    }

    //read rest of the file.
    if(read_file(&tag_manip_info) == e_failure) {
	dealloc_tag(&tag_manip_info);
	fclose(tag_manip_info.fptr_mp3_file);
	return 1;
    }

    //close mp3 file, which is in read mode, after reading it into mem.
    fclose(tag_manip_info.fptr_mp3_file);

    //edit or read tag, per user request.
    if(tag_manip_info.operation == o_edit) {

	if(edit_tag(&tag_manip_info, argv[1], argv[2]) == e_failure) {
	    print_error_message(STR_ERR_NO_TAG_FOUND);
	    dealloc_tag(&tag_manip_info);
	    return 1;
	}

	//write file, if edit succeeds.
	write_mp3_file(&tag_manip_info);
    } else {
	print_id3v2_tag_info(&tag_manip_info);
    }

    //deallocate memory.
    dealloc_tag(&tag_manip_info);
}
