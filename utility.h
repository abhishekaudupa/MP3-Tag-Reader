#ifndef UTILITY_H
#define UTILITY_H

#define MAX_MEM_ALLOC_ATTEMPTS 		500

#define STR_DEFAULT_TEMP_FILENAME 	"temp_dump_post_tag_edit.mp3"

Bool is_file_mp3(const char *const filename);

void *alloc_mem(const size_t n_bytes);

size_t get_int_from_syncsafe_int(const byte_t *syncsafe_int, const size_t n_bytes);

size_t get_int_from_size_bytes(const byte_t *const size_bytes, const size_t n_bytes);

Status read_from_file(void *buffer, FILE *fptr, size_t n_bytes);

Status write_to_file(const void *const buffer, FILE *const fptr, const size_t n_bytes);

Status seek_to(FILE *const fptr, const long offset, const int whence);

Status open_file(Tag_Manip_Info *const tag_manip_info, const char *const file_open_mode);

Status validate_syncsafe_int(const byte_t *const syncsafe_int, const size_t n_bytes);

Bit_Stat check_bit(const byte_t byte, int pos);

Status read_file(Tag_Manip_Info *const tag_manip_info);

size_t get_syncsafe_int_from_int(size_t size);

void update_syncsafe_int(byte_t *const syncsafe_size_bytes, size_t new_size, const size_t n_bytes);

Status rename_file(const char *const old_name, const char *const new_name);

Status remove_file(const char *const file_name);

#endif
