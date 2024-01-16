#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include "id3v2.h"
#include "utility.h"
#include "message.h"
#include "argument.h"

Status read_ID3v2_tag(Tag_Manip_Info *const tag_manip_info) {

    assert(tag_manip_info != NULL);
    assert(tag_manip_info->id3v2_tag != NULL);
    assert(tag_manip_info->fptr_mp3_file != NULL);
    assert(ftell(tag_manip_info->fptr_mp3_file) == 0);

    //read header.
    if(read_ID3v2_header(tag_manip_info) == e_failure)
    {
	print_error_message(STR_ERR_ID3v2_HEADER_READ_FAIL);
	return e_failure;
    }

    //validate header.
    if(validate_ID3v2_header(tag_manip_info) == e_failure)
    {
	print_error_message(STR_ERR_ID3v24_HEADER_VAL_FAIL);
	return e_failure;
    }

    //read and validate extended header, if present.
    if(check_bit(tag_manip_info->id3v2_tag->header.flags, 6) == b_set) {

	//read id3v23 extended header.
	if(tag_manip_info->tag_sub_version == ID3v2_3) {
	    //read ext. header.
	    if(read_ID3v23_ext_header(tag_manip_info) == e_failure) {
		print_error_message(STR_ERR_ID3v23_EHEADER_READ_FAIL);
		return e_failure;
	    }

	    //validate id3v24 extended header.
	    if(validate_ID3v23_ext_header(tag_manip_info) == e_failure) {
		print_error_message(STR_ERR_ID3v23_EHEADER_VAL_FAIL);
		return e_failure;
	    }
	}

	//read id3v24 extended header.
	if(tag_manip_info->tag_sub_version == ID3v2_4) {
	    
	    //read ext. header.
	    if(read_ID3v24_ext_header(tag_manip_info) == e_failure) {
		print_error_message(STR_ERR_ID3v24_EHEADER_READ_FAIL);
		return e_failure;
	    }

	    //validate id3v24 extended header.
	    if(validate_ID3v24_ext_header(tag_manip_info) == e_failure) {
		print_error_message(STR_ERR_ID3v24_EHEADER_VAL_FAIL);
		return e_failure;
	    }
	}	
    }

    //read all frames.
    while(ftell(tag_manip_info->fptr_mp3_file) < tag_manip_info->id3v2_tag->header.tag_size + 10) {
	if(read_ID3v2_frame(tag_manip_info) == e_failure) {
	    print_error_message(STR_ERR_FRAME_READ_FAIL);
	    return e_failure;
	}
    }

    return e_success;
}

Status read_ID3v2_header(Tag_Manip_Info *const tag_manip_info) {

    assert(tag_manip_info != NULL);
    assert(tag_manip_info->id3v2_tag != NULL);

    ID3v2_Header *header = &(tag_manip_info->id3v2_tag->header);

    //read into header struct members, simultaneously check for read failure.
    if((read_from_file(header->tag_id, 			tag_manip_info->fptr_mp3_file, ID3_TAG_ID_LEN) 			== e_failure)
	    || (read_from_file(header->version, 	tag_manip_info->fptr_mp3_file, ID3v2_VERSION_LEN) 		== e_failure)
	    || (read_from_file(&header->flags, 		tag_manip_info->fptr_mp3_file, 1) 				== e_failure)
	    || (read_from_file(header->syncsafe_size, 	tag_manip_info->fptr_mp3_file, ID3v2_HEADER_SYNCSAFE_INT_LEN) 	== e_failure))
	return e_failure;

    return e_success;
}

Status validate_ID3v2_header(Tag_Manip_Info *const tag_manip_info) {

    assert(tag_manip_info != NULL);
    assert(tag_manip_info->id3v2_tag != NULL);

    ID3v2_Header *header = &(tag_manip_info->id3v2_tag->header);

    //validate syncsafe integer
    if(validate_syncsafe_int(header->syncsafe_size, ID3v2_HEADER_SYNCSAFE_INT_LEN) == e_failure) {
	print_error_message(STR_ERR_SYNCSAFE_INT);
	return e_failure;
    }

    //calculate tag size from syncsafe integer.
    header->tag_size = get_int_from_syncsafe_int(header->syncsafe_size, ID3v2_HEADER_SYNCSAFE_INT_LEN);

    //record id3v24 tag size.
    tag_manip_info->id3v2_tag_size = tag_manip_info->id3v2_tag->header.tag_size
	+ ID3_TAG_ID_LEN + ID3v2_VERSION_LEN 
	+ ID3v2_HEADER_SYNCSAFE_INT_LEN 
	+ 1;

    return e_success;
}

Status read_ID3v24_ext_header(Tag_Manip_Info *const tag_manip_info) {

    assert(tag_manip_info != NULL);
    assert(tag_manip_info->id3v2_tag != NULL);

    ID3v24_Ext_Header *ext_header = &(tag_manip_info->id3v2_tag->ext_header_id3v24);

    //read extended header size and number of flag bytes.
    if((read_from_file(ext_header->syncsafe_size, tag_manip_info->fptr_mp3_file, ID3v2_EXT_HEADER_SYNCSAFE_INT_LEN) == e_failure)
	    || read_from_file(&(ext_header->n_flag_bytes), tag_manip_info->fptr_mp3_file, 1) == e_failure)
	return e_failure;

    return e_success;
}

Status read_ID3v23_ext_header(Tag_Manip_Info *const tag_manip_info) {

    assert(tag_manip_info != NULL);
    assert(tag_manip_info->id3v2_tag != NULL);

    ID3v23_Ext_Header *ext_header = &(tag_manip_info->id3v2_tag->ext_header_id3v23);

    //read extended header size and number of flag bytes.
    if(read_from_file(ext_header->syncsafe_size, tag_manip_info->fptr_mp3_file, ID3v2_EXT_HEADER_SYNCSAFE_INT_LEN) == e_failure
	    || read_from_file(ext_header->ext_flags, tag_manip_info->fptr_mp3_file, ID3v23_EXT_HEADER_FLAGS_LEN) == e_failure
	    || read_from_file(ext_header->padding_size, tag_manip_info->fptr_mp3_file, ID3v23_EXT_HEADER_PADDING_SYNCSAFE_INT_LEN) == e_failure)
	return e_failure;

    return e_success;
}

Status validate_ID3v24_ext_header(Tag_Manip_Info *const tag_manip_info) {

    assert(tag_manip_info != NULL);
    assert(tag_manip_info->id3v2_tag != NULL);

    ID3v24_Ext_Header *ext_header = &(tag_manip_info->id3v2_tag->ext_header_id3v24);

    //validate syncsafe integer.
    if(validate_syncsafe_int(ext_header->syncsafe_size, ID3v2_EXT_HEADER_SYNCSAFE_INT_LEN) == e_failure) {
	print_error_message(STR_ERR_SYNCSAFE_INT);
	return e_failure;
    }

    //calculate ext. header size from syncsafe integer.
    ext_header->size = get_int_from_syncsafe_int(ext_header->syncsafe_size, ID3v2_EXT_HEADER_SYNCSAFE_INT_LEN);

    //validate the ext. header size (it must, at minimum, be able to contain the header metadata).
    if(ext_header->size < ID3v2_EXT_HEADER_SYNCSAFE_INT_LEN + 1 + ext_header->n_flag_bytes) {
	print_error_message(STR_ERR_SYNCSAFE_INT);
	return e_failure;
    }

    //allocate mem. for ext. flags.
    ext_header->ext_flags = alloc_mem(ext_header->n_flag_bytes);
    if(!ext_header->ext_flags)
	return e_failure;

    //read ext. flags.
    if(read_from_file(ext_header->ext_flags, tag_manip_info->fptr_mp3_file, ext_header->n_flag_bytes) == e_failure)
	return e_failure;

    /* An advanced size validation could be done, which will be thorough.
     * 	- We verify if the syncsafe size matches with the data size indicated
     *	  by the flags above i.e if data exists for each of the flags set.
     *	  I'll implement this later, if I'm in the mood. For now, I'll take
     *	  it on faith that this won't be necessary. */

    //allocate mem. for header content.
    size_t ext_header_data_size = ext_header->size - ID3v2_EXT_HEADER_SYNCSAFE_INT_LEN - 1 - ext_header->n_flag_bytes;
    ext_header->content = alloc_mem(ext_header_data_size);
    if(!ext_header->content)
	return e_failure;

    //read header content from file.
    if(read_from_file(ext_header->content, tag_manip_info->fptr_mp3_file, ext_header_data_size))
	return e_failure;

    return e_success;
}

Status validate_ID3v23_ext_header(Tag_Manip_Info *const tag_manip_info) {

    assert(tag_manip_info != NULL);
    assert(tag_manip_info->id3v2_tag != NULL);

    ID3v23_Ext_Header *ext_header = &(tag_manip_info->id3v2_tag->ext_header_id3v23);

    //validate syncsafe integer.
    if(validate_syncsafe_int(ext_header->syncsafe_size, ID3v2_EXT_HEADER_SYNCSAFE_INT_LEN) == e_failure) {
	print_error_message(STR_ERR_SYNCSAFE_INT);
	return e_failure;
    }
    
    //validate padding syncsafe integer.
    if(validate_syncsafe_int(ext_header->padding_size, ID3v23_EXT_HEADER_PADDING_SYNCSAFE_INT_LEN) == e_failure) {
	print_error_message(STR_ERR_SYNCSAFE_INT);
	return e_failure;
    }

    return e_success;
}

Status read_ID3v2_frame(Tag_Manip_Info *const tag_manip_info) {

    assert(tag_manip_info != NULL);
    assert(tag_manip_info->id3v2_tag != NULL);

    ID3v2_Frame_Header frame_header;
    unsigned long curr_ind = ftell(tag_manip_info->fptr_mp3_file);

    //read frame header.
    if(read_from_file(frame_header.frame_id, 			tag_manip_info->fptr_mp3_file, ID3v2_FRAME_ID_LEN) 		== e_failure
	    || (read_from_file(frame_header.syncsafe_size, 	tag_manip_info->fptr_mp3_file, ID3v2_FRAME_SYNCSAFE_INT_LEN) 	== e_failure)
	    || (read_from_file(frame_header.flags, 		tag_manip_info->fptr_mp3_file, ID3v2_FRAME_FLAGS_LEN) 		== e_failure))
	return e_failure;

    if(tag_manip_info->tag_sub_version == ID3v2_4)
	//validate frame header.
	if(validate_ID3v2_frame_header(&frame_header) == e_failure) {
	    print_error_message(STR_ERR_FRAME_HDR_VAL_FAIL);
	    return e_failure;
	}

    if(tag_manip_info->tag_sub_version == ID3v2_4) {
	//calculate the frame content size from syncsafe integer.
	frame_header.content_size = get_int_from_syncsafe_int(frame_header.syncsafe_size, ID3v2_FRAME_SYNCSAFE_INT_LEN);
    }

    if(tag_manip_info->tag_sub_version == ID3v2_3) {
	//calculate the frame content size from size bytes.
	frame_header.content_size = get_int_from_size_bytes(frame_header.syncsafe_size, ID3v2_FRAME_SYNCSAFE_INT_LEN);
    }

    /* If this is an empty frame, then it means that the previous 
       frame was the last frame. If this is not the end of tag, 
       all bytes till the end of tag is padding. */
    if(frame_header.content_size == 0) {

	//seek to beginning of this 'frame'.
	if(seek_to(tag_manip_info->fptr_mp3_file, curr_ind, SEEK_SET) == e_failure)
	    return e_failure;

	//calculate and record padding size.
	tag_manip_info->id3v2_tag->padding.n_padding_bytes = (tag_manip_info->id3v2_tag->header.tag_size + 10) - curr_ind;

	//seek to end of padding.
	if(seek_to(tag_manip_info->fptr_mp3_file, curr_ind + tag_manip_info->id3v2_tag->padding.n_padding_bytes, SEEK_SET) == e_failure)
	    return e_failure;

	return e_success;
    }

    /* If this is not an empty frame, then add it to the linked list */

    //frame traverser to point to the correct node in the frame linked list.
    static ID3v2_Frame_List_Node *fln = NULL;
    static Bool init = 0;

    /* initialize the traverser to the first node if reading first frame
       otherwise allocate memeory for a new frame list node. */
    if(!init) {
	fln = &(tag_manip_info->id3v2_tag->first_frame);
	init = 1;
    } else {
	fln->next_node = alloc_mem(sizeof(*fln));
	fln = fln->next_node;
	if(!fln)
	    return e_failure;
	fln->next_node = NULL;
    }

    //copy header.
    memcpy(&(fln->frame.frame_header), &frame_header, sizeof(frame_header));

    //allocate mem. frame content.
    fln->frame.content = alloc_mem(frame_header.content_size);
    if(!fln->frame.content)
	return e_failure;

    //read frame content into mem.
    if(read_from_file(fln->frame.content, tag_manip_info->fptr_mp3_file, frame_header.content_size) == e_failure)
	return e_failure;

    return e_success;
}

Status validate_ID3v2_frame_header(ID3v2_Frame_Header *const frame_header) {

    assert(frame_header != NULL);

    //validate syncsafe integer.
    if(validate_syncsafe_int(frame_header->syncsafe_size, ID3v2_FRAME_SYNCSAFE_INT_LEN) == e_failure) {
	print_error_message(STR_ERR_SYNCSAFE_INT);
	return e_failure;
    }

    return e_success;
}

Status write_mp3_file(const Tag_Manip_Info *const tag_manip_info) {

    assert(tag_manip_info != NULL);

    //allocate memory.
    size_t id3v2_tag_content_size = tag_manip_info->id3v2_tag_size;
    byte_t *id3v2_tag_content = alloc_mem(id3v2_tag_content_size);
    if(!id3v2_tag_content)
	return e_failure;

    size_t iter = 0;
    //copy id3v2 tag into mem.
    if(copy_id3v2_tag_into_mem(tag_manip_info, id3v2_tag_content, &iter) == e_failure) {
	print_error_message(STR_ERR_TAG_COPY_FAIL);
	free(id3v2_tag_content);
	return e_failure;
    }

    //open a temporaty file for the intermediate mp3 file copy.
    FILE *mp3_temp_dump = fopen(STR_DEFAULT_TEMP_FILENAME, "wb");
    if(!mp3_temp_dump) {
	perror(NULL);
	free(id3v2_tag_content);
	return e_failure;
    }

    //write the tag.
    if(write_to_file(id3v2_tag_content, mp3_temp_dump, id3v2_tag_content_size) == e_failure) {
	fclose(mp3_temp_dump);
	remove_file(STR_DEFAULT_TEMP_FILENAME);
	return e_failure;
    }

    //write rest of the file.
    if(write_to_file(tag_manip_info->file_content, mp3_temp_dump, tag_manip_info->file_content_size) == e_failure) {
	fclose(mp3_temp_dump);
	remove_file(STR_DEFAULT_TEMP_FILENAME);
	return e_failure;
    }

    fclose(mp3_temp_dump);

    //delete old file.
    remove_file(tag_manip_info->mp3_filename);

    //rename temp file.
    rename_file(STR_DEFAULT_TEMP_FILENAME, tag_manip_info->mp3_filename);

    //free memory.
    free(id3v2_tag_content);
    return e_success;
}

Status copy_id3v2_tag_into_mem(const Tag_Manip_Info *const tag_manip_info, byte_t *const mem, size_t *const iter) {

    assert(tag_manip_info != NULL);
    assert(mem != NULL);
    assert(iter != NULL);

    //copy header
    if(copy_id3v2_header_into_mem(tag_manip_info, mem, iter) == e_failure) {
	print_error_message(STR_ERR_HEADER_COPY_FAIL);
	return e_failure;
    }

    //copy extended header, if it exists
    if(check_bit(tag_manip_info->id3v2_tag->header.flags, 6) == b_set) {

	if(tag_manip_info->tag_sub_version == ID3v2_4) {
	    if(copy_id3v24_ext_header_into_mem(tag_manip_info, mem, iter) == e_failure) {
		print_error_message(STR_ERR_EXT_HEADER_COPY_FAIL);
		return e_failure;
	    }
	}

	if(tag_manip_info->tag_sub_version == ID3v2_3) {
	    if(copy_id3v23_ext_header_into_mem(tag_manip_info, mem, iter) == e_failure) {
		print_error_message(STR_ERR_EXT_HEADER_COPY_FAIL);
		return e_failure;
	    }
	}
    }

    //copy all frames
    if(copy_id3v2_frames_into_mem(tag_manip_info, mem, iter) == e_failure) {
	print_error_message(STR_ERR_FRAME_COPY_FAIL);
	return e_failure;
    }

    //set padding to 0.
    for(register size_t j = 0; j < tag_manip_info->id3v2_tag->padding.n_padding_bytes; ++j)
	mem[(*iter)++] = 0;

    return e_success;
}

Status copy_id3v2_header_into_mem(const Tag_Manip_Info *const tag_manip_info, byte_t *const mem, size_t *const iter) {

    assert(tag_manip_info != NULL);
    assert(tag_manip_info->id3v2_tag != NULL);
    assert(iter != NULL);
    assert(mem != NULL);

    ID3v2_Header header = tag_manip_info->id3v2_tag->header;

    //copy tag id
    for(register size_t j = 0; j < ID3_TAG_ID_LEN; ++j, ++*iter)
	mem[*iter] = header.tag_id[j];

    //copy version
    for(register size_t j = 0; j < ID3v2_VERSION_LEN; ++j, ++*iter)
	mem[*iter] = header.version[j];

    //copy flags
    mem[(*iter)++] = header.flags;

    //copy syncsafe size
    for(register size_t j = 0; j < ID3v2_HEADER_SYNCSAFE_INT_LEN; ++j, ++*iter)
	mem[*iter] = header.syncsafe_size[j];

    return e_success;
}

Status copy_id3v24_ext_header_into_mem(const Tag_Manip_Info *const tag_manip_info, byte_t *const mem, size_t *const iter) {

    assert(tag_manip_info != NULL);
    assert(tag_manip_info->id3v2_tag != NULL);
    assert(iter != NULL);
    assert(mem != NULL);

    ID3v24_Ext_Header ext_header = tag_manip_info->id3v2_tag->ext_header_id3v24;

    //copy syncsafe size;
    for(register size_t j = 0; j < ID3v2_EXT_HEADER_SYNCSAFE_INT_LEN; ++j, ++*iter)
	mem[*iter] = ext_header.syncsafe_size[j];

    //copy num. of flag bytes.
    mem[(*iter)++] = ext_header.n_flag_bytes;

    //copy flags.
    for(register size_t j = 0; j < ext_header.n_flag_bytes; ++j, ++*iter)
	mem[*iter] = ext_header.ext_flags[j];

    //copy contents.
    for(register size_t j = 0; j < ext_header.size; ++j, ++*iter)
	mem[*iter] = ext_header.content[j];

    return e_success;
}

Status copy_id3v23_ext_header_into_mem(const Tag_Manip_Info *const tag_manip_info, byte_t *const mem, size_t *const iter) {

    assert(tag_manip_info != NULL);
    assert(tag_manip_info->id3v2_tag != NULL);
    assert(iter != NULL);
    assert(mem != NULL);

    ID3v23_Ext_Header ext_header = tag_manip_info->id3v2_tag->ext_header_id3v23;

    //copy syncsafe size;
    for(register size_t j = 0; j < ID3v2_EXT_HEADER_SYNCSAFE_INT_LEN; ++j, ++*iter)
	mem[*iter] = ext_header.syncsafe_size[j];

    //copy flag bytes.
    mem[(*iter)++] = ext_header.ext_flags[0];
    mem[(*iter)++] = ext_header.ext_flags[1];

    //copy padding size.
    for(register size_t j = 0; j < ID3v23_EXT_HEADER_PADDING_SYNCSAFE_INT_LEN; ++j, ++*iter)
	mem[*iter] = ext_header.padding_size[j];

    return e_success;
}

Status copy_id3v2_frames_into_mem(const Tag_Manip_Info *const tag_manip_info, byte_t *const mem, size_t *const iter) {

    assert(tag_manip_info != NULL);
    assert(tag_manip_info->id3v2_tag != NULL);
    assert(iter != NULL);
    assert(mem != NULL);

    //frames linked list traverser.
    ID3v2_Frame_List_Node *trav_fln = &(tag_manip_info->id3v2_tag->first_frame);

    //go through each node in the LL.
    while(trav_fln) {
	ID3v2_Frame frame = trav_fln->frame;
	ID3v2_Frame_Header frame_header = frame.frame_header;

	//copy frame id.
	for(register size_t j = 0; j < ID3v2_FRAME_ID_LEN; ++j, ++*iter)
	    mem[*iter] = frame_header.frame_id[j];

	//copy frame syncsafe size.
	for(register size_t j = 0; j < ID3v2_FRAME_SYNCSAFE_INT_LEN; ++j, ++*iter)
	    mem[*iter] = frame_header.syncsafe_size[j];

	//copy flags.
	for(register size_t j = 0; j < ID3v2_FRAME_FLAGS_LEN; ++j, ++*iter)
	    mem[*iter] = frame_header.flags[j];

	//copy content.
	for(register size_t j = 0; j < frame_header.content_size; ++j, ++*iter)
	    mem[*iter] = frame.content[j];

	trav_fln = trav_fln->next_node;
    }

    return e_success;
}

void print_id3v2_tag_info(const Tag_Manip_Info *const tag_manip_info) {

    assert(tag_manip_info != NULL);

    //tag ids to read and their display names.
    char *tag_heads[] = { 
	"Title    : ", 
	"Track    : ", 
	"Artist   : ", 
	"Album    : ", 
	"Genre    : ", 
	"Year     : ", 
	"Comments : ", 
	NULL };

    char *id3v24_tag_ids[] = { "TIT2", "TRCK", "TPE1", "TALB", "TCON", "TDRC", "COMM", NULL };
    char *id3v23_tag_ids[] = { "TIT2", "TRCK", "TPE1", "TALB", "TCON", "TYER", "COMM", NULL };

    char **tag_ids = NULL;

    //chose frame ids based on the tag version.
    if(tag_manip_info->tag_sub_version == ID3v2_3)
	tag_ids = id3v23_tag_ids;

    if(tag_manip_info->tag_sub_version == ID3v2_4)
	tag_ids = id3v24_tag_ids;

    //struct array to point to the displayable content.
    struct {
	byte_t *content;
	size_t content_size;
    } tag_info[7];

    for(register size_t i = 0; i < 7; ++i)
	tag_info[i].content = NULL;

    //frames linked list traverser.
    ID3v2_Frame_List_Node *trav_fln = &(tag_manip_info->id3v2_tag->first_frame);

    //go through each node in the LL.
    while(trav_fln) {
	ID3v2_Frame frame = trav_fln->frame;
	ID3v2_Frame_Header frame_header = frame.frame_header;

	//acquire relevant contents.
	for(register size_t i = 0; i < 7; ++i) {
	    if(strncmp((char*)frame_header.frame_id, tag_ids[i], ID3v2_FRAME_ID_LEN) == 0) {
		tag_info[i].content = frame.content;
		tag_info[i].content_size = frame_header.content_size;
	    }
	}

	trav_fln = trav_fln->next_node;
    }

    //display tag version.
    if(tag_manip_info->tag_sub_version == ID3v2_4)
	print_message("Tag Version: ID3v2.4\n");

    if(tag_manip_info->tag_sub_version == ID3v2_3)
	print_message("Tag Version: ID3v2.3\n");

    //display title, track, artist, album and genre.
    for(register size_t i = 0; i < 5; ++i) {
	print_message(tag_heads[i]);
	if(tag_info[i].content)
	    print_text_frame_content(tag_info[i].content, tag_info[i].content_size, 0, 0);
	else
	    print_message("None\n");
    }

    //display year.
    print_message(tag_heads[5]);
    if(tag_info[5].content)
	print_text_frame_content(tag_info[5].content, tag_info[5].content_size, 4, 0);
    else
	print_message("None\n");

    //display comments.
    print_message(tag_heads[6]);
    if(tag_info[6].content)
	print_text_frame_content(tag_info[6].content, tag_info[6].content_size, 0, 3);
    else
	print_message("None\n");

}

Status edit_id3v2_tag(Tag_Manip_Info *const tag_manip_info, const char *const edit_option, const char *const new_content) {

    assert(tag_manip_info != NULL);
    assert(new_content != NULL);
    assert(edit_option != NULL);

    //get frame id using the edit option.
    char *frame_id = NULL;

    if(strcmp(ARG_MOD_TITLE, edit_option) == 0)
	frame_id = "TIT2";

    if(strcmp(ARG_MOD_TRACK, edit_option) == 0)
	frame_id = "TRCK";

    if(strcmp(ARG_MOD_ARTIST, edit_option) == 0)
	frame_id = "TPE1";

    if(strcmp(ARG_MOD_ALBUM, edit_option) == 0)
	frame_id = "TALB";

    if(strcmp(ARG_MOD_YEAR, edit_option) == 0) {
	if(tag_manip_info->tag_sub_version == ID3v2_4)
	    frame_id = "TDRC";

	if(tag_manip_info->tag_sub_version == ID3v2_3)
	    frame_id = "TYER";
    }

    if(strcmp(ARG_MOD_GENRE, edit_option) == 0)
	frame_id = "TCON";

    if(strcmp(ARG_MOD_COMMENT, edit_option) == 0) {
	frame_id = "COMM";
    }

    assert(frame_id != NULL);

    ID3v2_Header *header = &(tag_manip_info->id3v2_tag->header);

    //frames linked list traverser.
    ID3v2_Frame_List_Node *trav_fln = &(tag_manip_info->id3v2_tag->first_frame);

    //go through each node in the LL.
    while(trav_fln) {
	ID3v2_Frame *frame = &(trav_fln->frame);
	ID3v2_Frame_Header *frame_header = &(frame->frame_header);

	//if the necessary tag is found, edit it's contents.
	if(strncmp((char*)frame_header->frame_id, frame_id, ID3v2_FRAME_ID_LEN) == 0) {

	    size_t new_content_size = 0;

	    if(strncmp(frame_id, "COMM", ID3v2_FRAME_ID_LEN) == 0) {
		//get the new content's size (string size + text encoding byte + 3 language identifier bytes).
		new_content_size = strlen(new_content) + 5;

		//edit the content.
		if(edit_comment_frame_content(&frame->content, frame_header->content_size, new_content, new_content_size) == e_failure) {
		    return e_failure;
		}
	    } else {
		//get the new content's size (string size + text encoding byte).
		new_content_size = strlen(new_content) + 2;

		//edit the content.
		if(edit_text_frame_content(&frame->content, frame_header->content_size, new_content, new_content_size) == e_failure) {
		    return e_failure;
		}
	    }

	    //get the new tag size.
	    size_t new_tag_size = header->tag_size - frame_header->content_size + new_content_size;

	    //change syncsafe size of frame.
	    update_syncsafe_int(frame_header->syncsafe_size, get_syncsafe_int_from_int(new_content_size), ID3v2_FRAME_SYNCSAFE_INT_LEN);
	    frame_header->content_size = get_int_from_syncsafe_int(frame_header->syncsafe_size, ID3v2_FRAME_SYNCSAFE_INT_LEN);

	    //change syncsafe size of id3 header.
	    update_syncsafe_int(header->syncsafe_size, get_syncsafe_int_from_int(new_tag_size), ID3v2_HEADER_SYNCSAFE_INT_LEN);
	    header->tag_size = get_int_from_syncsafe_int(header->syncsafe_size, ID3v2_HEADER_SYNCSAFE_INT_LEN);

	    return e_success;
	}

	trav_fln = trav_fln->next_node;
    }

    return e_failure;
}

void print_text_frame_content(const byte_t *const content, const size_t content_size, const size_t max_chars_to_print, const size_t offset) {

    assert(content != NULL);

    //first byte of content is text encoding.
    const byte_t text_enc = content[0];

    //if UTF-16, ignore first two bytes (BOM) after text enc.
    register size_t i = (text_enc == TXT_ENC_UTF_16)? (1 + UTF_16_BOM_SIZE): 1;

    //error check.
    if(text_enc < TXT_ENC_ISO_8859_1 || text_enc > TXT_ENC_UTF_8)
	print_error_message(STR_WAR_UNKNOWN_TEXT_ENC);

    //offset used to print contents from a certain offset (excludes text enc. byte offset).
    i += offset;

    //print contents.
    if(!max_chars_to_print) {
	for(; i < content_size; ++i)
	    putchar(content[i]);
    } else {
	for(register size_t j = 0; j < max_chars_to_print; ++j, ++i)
	    putchar(content[i]);
    }

    print_message("\n");
}

Status edit_text_frame_content(byte_t **content, const size_t content_size, const char *const new_content, const size_t new_content_size) {

    assert(content != NULL);
    assert(new_content != NULL);

    //get new memory allocated if new content is larger.
    if(new_content_size > content_size) {
	void *new_ptr = alloc_mem(new_content_size);
	if(!new_ptr) {
	    print_error_message(STR_ERR_MEM_ALLOC_FAIL);
	    return e_failure;
	}

	free(*content);
	*content = new_ptr;
    }

    //encode it as a ISO-8859-1 string.
    (*content)[0] = TXT_ENC_ISO_8859_1;

    //copy the new content.
    memcpy(*content + 1, new_content, new_content_size - 1);

    return e_success;
}

Status edit_comment_frame_content(byte_t **content, const size_t content_size, const char *const new_content, const size_t new_content_size) {

    assert(content != NULL);
    assert(new_content != NULL);

    printf("Content size: %ld and new content size: %ld\n", content_size, new_content_size);
    print_text_frame_content(*content, content_size, 0, 0);

    //get new memory allocated if new content is larger.
    if(new_content_size > content_size) {
	void *new_ptr = alloc_mem(new_content_size);
	if(!new_ptr) {
	    print_error_message(STR_ERR_MEM_ALLOC_FAIL);
	    return e_failure;
	}

	free(*content);
	*content = new_ptr;
    }


    //encode it as a ISO-8859-1 string.
    (*content)[0] = TXT_ENC_ISO_8859_1;

    //set language to english.
    (*content)[1] = 'e';
    (*content)[2] = 'n';
    (*content)[3] = 'g';


    //copy the new content.
    memcpy(*content + 4, new_content, new_content_size - 4);

    return e_success;

}
