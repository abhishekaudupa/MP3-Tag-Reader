#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include "argument.h"
#include "types.h"
#include "utility.h"
#include "message.h"

Bool is_file_mp3(const char *const filename) {

    assert(filename != NULL);

    const char *last_dot = NULL;
    const char *cur_dot = filename;

    while(1) {
	cur_dot = strchr(cur_dot, '.');
	if(cur_dot) {
	    last_dot = cur_dot;
	    ++cur_dot;
	} else {
	    break;
	}
    }

    if(last_dot)
	return strcmp(last_dot, ".mp3")? e_false: e_true;
    return e_false;
}

void *alloc_mem(const size_t n_bytes) {
    void *mem_bytes = malloc(n_bytes);
    for(register size_t i = 0; ((i < MAX_MEM_ALLOC_ATTEMPTS) && !mem_bytes); ++i)
	mem_bytes = malloc(n_bytes);
    if(!mem_bytes)
	print_error_message(STR_ERR_MEM_ALLOC_FAIL);
    return mem_bytes;
}

size_t get_int_from_syncsafe_int(const byte_t *syncsafe_int, const size_t n_bytes) {

    assert(syncsafe_int != NULL);

    size_t s_int = 0;
    for(register size_t k = 0; k < n_bytes; ++k)
	for(register size_t i = 7 * k; i < 7 * k + 7; ++i)
	    s_int += ((syncsafe_int[n_bytes - k - 1] >> (i - 7 * k)) & 1) << i;
    return s_int;
}

size_t get_int_from_size_bytes(const byte_t *const size_bytes, const size_t n_bytes) {
    
    assert(size_bytes != NULL);

    size_t s_int = 0;
    unsigned char *temp = (unsigned char*)&s_int;

    for(register size_t i = 0; i < n_bytes; ++i) {
	temp[i] = size_bytes[n_bytes - 1 - i];
    }
    return s_int;
}

Status read_from_file(void *buffer, FILE *fptr, const size_t n_bytes) {

    assert(buffer != NULL && fptr != NULL);

    fread(buffer, n_bytes, 1, fptr);
    if(ferror(fptr)) {
	perror(NULL);
	return e_failure;
    }

    return e_success;
}

Status write_to_file(const void *const buffer, FILE *const fptr, const size_t n_bytes) {

    assert(buffer != NULL);
    assert(fptr != NULL);

    fwrite(buffer, n_bytes, 1, fptr);
    if(ferror(fptr)) {
	perror(NULL);
	return e_failure;
    }

    return e_success;
}

Status seek_to(FILE *const fptr, const long offset, const int whence) {

    assert(fptr != NULL);

    fseek(fptr, offset, whence);
    if(ferror(fptr)) {
	perror("File seek failed");
	return e_failure;
    }
    return e_success;
}

Status open_file(Tag_Manip_Info *const tag_manip_info, const char *const file_open_mode) {

    assert(tag_manip_info != NULL && file_open_mode != NULL);

    tag_manip_info->fptr_mp3_file = fopen(tag_manip_info->mp3_filename, file_open_mode);

    if(!tag_manip_info->fptr_mp3_file) {
	perror("Failed to open the file");
	return e_failure;
    }

    return e_success;
}

Status validate_syncsafe_int(const byte_t *const syncsafe_int, const size_t n_bytes) {
   
    assert(syncsafe_int != NULL);

    for(register size_t i = 0; i < n_bytes; ++i)
	if(syncsafe_int[i] >= 0x80)
	    return e_failure;
    return e_success;
}

Bit_Stat check_bit(const byte_t byte, int pos) {

    assert(pos >= 0 && pos <= 7);

    return (byte >> pos) & 0x01;
}

Status read_file(Tag_Manip_Info *const tag_manip_info) {

    assert(tag_manip_info != NULL);
    assert(tag_manip_info->fptr_mp3_file != NULL);

    //get size of the content
    unsigned long curr = ftell(tag_manip_info->fptr_mp3_file);
    size_t content_size = 0;
    char c;
    while(1) {
	if(read_from_file(&c, tag_manip_info->fptr_mp3_file, 1) == e_failure)
	    return e_failure;
	if(feof(tag_manip_info->fptr_mp3_file))
	    break;
	++content_size;
    }
    if(seek_to(tag_manip_info->fptr_mp3_file, curr, SEEK_SET) == e_failure)
	return e_failure;

    //allocate mem.
    tag_manip_info->file_content = alloc_mem(content_size);
    if(!tag_manip_info->file_content)
	return e_failure;

    //copy contents into mem.
    if(read_from_file(tag_manip_info->file_content, tag_manip_info->fptr_mp3_file, content_size) == e_failure)
	return e_failure;

    //record content size.
    tag_manip_info->file_content_size = content_size;

    return e_success;
}

size_t get_syncsafe_int_from_int(size_t size) {
    size_t n_byte_count = ceil(log(size + 1) / 4.852030264);
    if(n_byte_count > sizeof(size_t)) {
	print_error_message(STR_ERR_SYNCSAFE_CONV);
	return 0;
    }

    size_t syncsafe_int = 0;
    size_t mask = 0x7F;

    for(register size_t i = 0; i < sizeof(size_t); ++i) {
	syncsafe_int |= (size << i) & (mask << (8 * i));
    }

    return syncsafe_int;
}

void update_syncsafe_int(byte_t *const syncsafe_size_bytes, size_t new_size, const size_t n_bytes) {

    assert(syncsafe_size_bytes != NULL);
    assert(n_bytes <= sizeof(new_size));

    const byte_t *ptr = (byte_t*)&new_size;
    assert(validate_syncsafe_int(ptr, n_bytes) == e_success);

    for(register size_t i = 0; i < n_bytes; ++i) {
	syncsafe_size_bytes[i] = ptr[n_bytes - i - 1];
    }
}

Status rename_file(const char *const old_name, const char *const new_name) {

    assert(old_name != NULL);
    assert(new_name != NULL);

    if(rename(old_name, new_name) != 0) {
	print_error_message(STR_ERR_FILE_RENAME_FAIL);
	return e_failure;
    }

    return e_success;
}

Status remove_file(const char *const file_name) {

    assert(file_name != NULL);

    if(remove(file_name) != 0) {
	print_error_message(STR_ERR_FILE_REMOVE_FAIL);
	return e_failure;
    }
    
    return e_success;
}
