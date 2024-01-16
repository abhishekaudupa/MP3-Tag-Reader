#ifndef ID3V2_H
#define ID3V2_H

#include "types.h"

Status read_ID3v2_tag(Tag_Manip_Info *const tag_manip_info);

Status read_ID3v2_header(Tag_Manip_Info *const tag_manip_info);

Status validate_ID3v2_header(Tag_Manip_Info *const tag_manip_info);

Status read_ID3v24_ext_header(Tag_Manip_Info *const tag_manip_info);

Status read_ID3v23_ext_header(Tag_Manip_Info *const tag_manip_info);

Status validate_ID3v24_ext_header(Tag_Manip_Info *const tag_manip_info);

Status validate_ID3v23_ext_header(Tag_Manip_Info *const tag_manip_info);

Status read_ID3v2_frame(Tag_Manip_Info *const tag_manip_info);

Status validate_ID3v2_frame_header(ID3v2_Frame_Header *const frame_header);

Status write_mp3_file(const Tag_Manip_Info *const tag_manip_info);

Status copy_id3v2_tag_into_mem(const Tag_Manip_Info *const tag_manip_info, byte_t *const mem, size_t *const iter);

Status copy_id3v2_header_into_mem(const Tag_Manip_Info *const tag_manip_info, byte_t *const mem, size_t *const iter);

Status copy_id3v23_ext_header_into_mem(const Tag_Manip_Info *const tag_manip_info, byte_t *const mem, size_t *const iter);

Status copy_id3v24_ext_header_into_mem(const Tag_Manip_Info *const tag_manip_info, byte_t *const mem, size_t *const iter);

Status copy_id3v2_frames_into_mem(const Tag_Manip_Info *const tag_manip_info, byte_t *const mem, size_t *const iter);

void print_id3v2_tag_info(const Tag_Manip_Info *const tag_manip_info);

Status edit_id3v2_tag(Tag_Manip_Info *const tag_manip_info, const char *const frame_id, const char *const new_content);

void print_text_frame_content(const byte_t *const content, const size_t content_size, const size_t max_chars_to_print, const size_t offset);

Status edit_text_frame_content(byte_t **content, const size_t content_size, const char *const new_content, const size_t new_content_size);

Status edit_comment_frame_content(byte_t **content, const size_t content_size, const char *const new_content, const size_t new_content_size);

#endif
