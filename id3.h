#ifndef ID3_H
#define ID3_H

#include "types.h"

Status validate_ID3_tag(Tag_Manip_Info *const tag_manip_info);

Status get_ID3v2_sub_version(Tag_Manip_Info *const tag_manip_info);

Status get_ID3v1_sub_version(Tag_Manip_Info *const tag_manip_info);

Status validate_sub_version(Tag_Manip_Info *const tag_manip_info);

Status alloc_mem_for_tag(Tag_Manip_Info *const tag_manip_info);

void dealloc_tag(Tag_Manip_Info *const tag_manip_info);

void dealloc_id3v24_tag(ID3v2_Tag *const id3v24_tag);

void dealloc_fln(ID3v2_Frame_List_Node *const fln);

Status read_id3_tag(Tag_Manip_Info *const tag_manip_info);

Status edit_tag(Tag_Manip_Info *const tag_manip_info, const char *const oper_option, const char *const new_content);

#endif
