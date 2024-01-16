#include <stdlib.h>
#include <assert.h>
#include "id3.h"
#include "message.h"
#include "utility.h"
#include "id3v2.h"

Status validate_ID3_tag(Tag_Manip_Info *const tag_manip_info) {

    assert(tag_manip_info != NULL);

    char tag_id[ID3_TAG_ID_LEN];

    if(read_from_file(tag_id, tag_manip_info->fptr_mp3_file, ID3_TAG_ID_LEN) == e_failure)
	return e_failure;

    rewind(tag_manip_info->fptr_mp3_file);

    //check for ID3v2 and store.
    if(tag_id[0] == 'I' && tag_id[1] == 'D' && tag_id[2] == '3') {
	tag_manip_info->tag_version = ID3v2;
	return e_success;
    }

    //check for ID3v1 and store.
    if(tag_id[0] == 'T' && tag_id[1] == 'A' && tag_id[2] == 'G') {
	tag_manip_info->tag_version = ID3v1;
	return e_success;
    }

    print_error_message(STR_ERR_ABSENT_TAG);
    tag_manip_info->tag_version = ID3_invalid;
    return e_failure;
}

Status get_ID3v2_sub_version(Tag_Manip_Info *const tag_manip_info) {

    assert(tag_manip_info != NULL);
    assert(tag_manip_info->fptr_mp3_file != NULL);

    FILE *fptr_mp3_file = tag_manip_info->fptr_mp3_file;
    assert(ftell(fptr_mp3_file) == 0);

    byte_t sv_byte;
    
    if(seek_to(fptr_mp3_file, 3, SEEK_SET) == e_failure)
	return e_failure;

    if(read_from_file(&sv_byte, fptr_mp3_file, 1) == e_failure)
	return e_failure;

    rewind(fptr_mp3_file);

    switch(sv_byte) {

	case 3:
	    tag_manip_info->tag_sub_version = ID3v2_3;
	    return e_success;

	case 4:
	    tag_manip_info->tag_sub_version = ID3v2_4;
	    return e_success;

	default:
	    print_error_message(STR_ERR_UNKNOWN_ID3V2);
	    return e_failure;
    }

    return e_success;
}

Status get_ID3v1_sub_version(Tag_Manip_Info *const tag_manip_info) {

    assert(tag_manip_info != NULL);
    assert(tag_manip_info->fptr_mp3_file != NULL);
    
    //to be implemented
    print_error_message("ID3v1 reading not implemented yet.\n");
    return e_failure;
}

Status validate_sub_version(Tag_Manip_Info *const tag_manip_info) {

    assert(tag_manip_info != NULL);
    assert(tag_manip_info->tag_version != ID3_invalid);

    switch(tag_manip_info->tag_version) {

	case ID3v1:
	    return get_ID3v1_sub_version(tag_manip_info);

	case ID3v2:
	    return get_ID3v2_sub_version(tag_manip_info);

	default:
	    return e_failure;
    }
    return e_failure;
}

Status alloc_mem_for_tag(Tag_Manip_Info *const tag_manip_info) {

    assert(tag_manip_info != NULL);

    switch(tag_manip_info->tag_sub_version) {

	case ID3v1_1:
	    //tbd
	    return e_failure;	    

	case ID3v2_3:
	    //allocate mem.
	    tag_manip_info->id3v2_tag = alloc_mem(sizeof(*(tag_manip_info->id3v2_tag)));
	    ID3v2_Tag *id3v23_tag = tag_manip_info->id3v2_tag;
	    if(!id3v23_tag)
		return e_failure;
	    //well define object.
	    id3v23_tag->first_frame.next_node = NULL;
	    //return
	    return e_success;

	case ID3v2_4:
	    //allocate mem.
	    tag_manip_info->id3v2_tag = alloc_mem(sizeof(*(tag_manip_info->id3v2_tag)));
	    ID3v2_Tag *id3v24_tag = tag_manip_info->id3v2_tag;
	    if(!id3v24_tag)
		return e_failure;
	    //well define object.
	    id3v24_tag->ext_header_id3v24.ext_flags = NULL;
	    id3v24_tag->ext_header_id3v24.content = NULL;
	    id3v24_tag->first_frame.next_node = NULL;
	    //return
	    return e_success;

	default:
	    return e_failure;
    }
    return e_failure;
}


void dealloc_tag(Tag_Manip_Info *const tag_manip_info) {
    
    assert(tag_manip_info != NULL);
    assert(tag_manip_info->tag_version != ID3_invalid);
    assert(tag_manip_info->tag_sub_version != ID3_sv_invalid);

    switch(tag_manip_info->tag_sub_version) {

	case ID3v1_1:
	    //tbd
	    print_error_message("We shouldn't be here.\n");
	    assert(0);

	case ID3v2_3:
	    dealloc_id3v24_tag(tag_manip_info->id3v2_tag);
	    return;

	case ID3v2_4:
	    dealloc_id3v24_tag(tag_manip_info->id3v2_tag);
	    return;

	default:
	    print_error_message("We shouldn't be here.\n");
	    assert(0);
	    return;
    }
    free(tag_manip_info->file_content);
}

void dealloc_id3v24_tag(ID3v2_Tag *const id3v24_tag) {

    assert(id3v24_tag != NULL);

    free(id3v24_tag->ext_header_id3v24.ext_flags);
    free(id3v24_tag->ext_header_id3v24.content);
    dealloc_fln(id3v24_tag->first_frame.next_node);
}

void dealloc_id3v23_tag(ID3v2_Tag *const id3v23_tag) {

    assert(id3v23_tag != NULL);
    dealloc_fln(id3v23_tag->first_frame.next_node);
}

void dealloc_fln(ID3v2_Frame_List_Node *const fln) {

    if(!fln)
	return;

    ID3v2_Frame_List_Node *trav = fln;
    ID3v2_Frame_List_Node *foll = NULL;

    while(trav) {
	foll = trav;
	trav = trav->next_node;
	free(foll);
    }
}

Status read_id3_tag(Tag_Manip_Info *const tag_manip_info) {

    assert(tag_manip_info != NULL);
    assert(tag_manip_info->tag_version != ID3_invalid);
    assert(tag_manip_info->tag_sub_version != ID3_sv_invalid);

    switch(tag_manip_info->tag_sub_version) {

	case ID3v1_1:
	    //tbd
	    print_error_message("We shouldn't be here.\n");
	    assert(0);

	case ID3v2_3:
	    return read_ID3v2_tag(tag_manip_info);

	case ID3v2_4:
	    return read_ID3v2_tag(tag_manip_info);

	default:
	    print_error_message("We shouldn't be here.\n");
	    assert(0);
	    return e_failure;
    }
    return e_failure;
}

Status edit_tag(Tag_Manip_Info *const tag_manip_info, const char *const oper_option, const char *const new_content) {

    assert(tag_manip_info != NULL);
    assert(new_content != NULL);

    switch(tag_manip_info->tag_sub_version) {

	case ID3v1_1:
	    //tbd
	    print_error_message("We shouldn't be here.\n");
	    assert(0);
	
	case ID3v2_3:
	    return edit_id3v2_tag(tag_manip_info, oper_option, new_content);
	
	case ID3v2_4:
	    return edit_id3v2_tag(tag_manip_info, oper_option, new_content);
	
	default:
	    print_error_message("We shouldn't be here.\n");
	    assert(0);
	    return e_failure;
    }

    return e_failure;
}
