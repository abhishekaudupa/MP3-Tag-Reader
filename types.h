#ifndef TYPES_H
#define TYPES_H

#include <stdio.h>

#define ID3_TAG_ID_LEN 					3
#define ID3v2_VERSION_LEN 				2
#define ID3v2_HEADER_SYNCSAFE_INT_LEN 			4

#define ID3v2_EXT_HEADER_SYNCSAFE_INT_LEN 		4
#define ID3v23_EXT_HEADER_FLAGS_LEN			2
#define ID3v23_EXT_HEADER_PADDING_SYNCSAFE_INT_LEN 	4

#define ID3v2_FRAME_ID_LEN 				4
#define ID3v2_FRAME_SYNCSAFE_INT_LEN 			4
#define ID3v2_FRAME_FLAGS_LEN 				2

#define TXT_ENC_ISO_8859_1				0x00
#define TXT_ENC_UTF_16					0x01
#define TXT_ENC_UTF_16BE				0x02
#define TXT_ENC_UTF_8					0x03

#define UTF_16_BOM_SIZE					2

typedef unsigned char byte_t;
typedef byte_t Flags;

typedef enum _Bool_ {
    e_false,
    e_true
} Bool;

typedef enum _Status {
    e_failure,
    e_success
} Status;

typedef enum _Bit_Stat {
    b_unset,
    b_set
} Bit_Stat;

typedef enum _Validity {
    e_invalid,
    e_valid
} Validity;

typedef enum _Operation {
    o_read,
    o_edit,
    o_help,
    o_invalid
} Operation;

typedef enum _Tag_Version {
    ID3v1,
    ID3v2,
    ID3_invalid
} Tag_Version;

typedef enum _Tag_Sub_Version {
    ID3v1_0,
    ID3v1_1,
    ID3v2_3,
    ID3v2_4,
    ID3_sv_invalid
} Tag_Sub_Version;

typedef struct _ID3v2_Header {
    byte_t tag_id[ID3_TAG_ID_LEN];
    byte_t version[ID3v2_VERSION_LEN];
    Flags flags;
    byte_t syncsafe_size[ID3v2_HEADER_SYNCSAFE_INT_LEN];
    size_t tag_size;
} ID3v2_Header;

typedef struct _ID3v23_Ext_Header {
    byte_t syncsafe_size[ID3v2_EXT_HEADER_SYNCSAFE_INT_LEN];
    Flags ext_flags[ID3v23_EXT_HEADER_FLAGS_LEN];
    byte_t padding_size[ID3v23_EXT_HEADER_PADDING_SYNCSAFE_INT_LEN];
} ID3v23_Ext_Header;

typedef struct _ID3v24_Ext_Header {
    byte_t syncsafe_size[ID3v2_EXT_HEADER_SYNCSAFE_INT_LEN];
    byte_t n_flag_bytes;
    Flags *ext_flags;
    byte_t *content;
    size_t size;
} ID3v24_Ext_Header;

typedef struct _ID3v2_Frame_Header {
    byte_t frame_id[ID3v2_FRAME_ID_LEN];
    byte_t syncsafe_size[ID3v2_FRAME_SYNCSAFE_INT_LEN];
    Flags flags[ID3v2_FRAME_FLAGS_LEN];
    size_t content_size;
    Validity frame_id_validity;
} ID3v2_Frame_Header;

typedef struct _ID3v2_Frame {
    ID3v2_Frame_Header frame_header;
    byte_t *content;
} ID3v2_Frame;

typedef struct _ID3v2_Frame_List_Node {
    ID3v2_Frame frame;
    struct _ID3v2_Frame_List_Node *next_node;
} ID3v2_Frame_List_Node;

typedef struct _ID3v2_Padding {
    size_t n_padding_bytes;
} ID3v2_Padding;

typedef struct _ID3v2_Tag {
    ID3v2_Header header;
    ID3v24_Ext_Header ext_header_id3v24;
    ID3v23_Ext_Header ext_header_id3v23;
    ID3v2_Frame_List_Node first_frame;
    ID3v2_Padding padding;
    ID3v2_Header footer;
} ID3v2_Tag;

typedef struct _Tag_Manip_Info {
    FILE *fptr_mp3_file;
    char *mp3_filename;
    Operation operation;
    char *oper_option;
    Tag_Version tag_version;
    Tag_Sub_Version tag_sub_version;
    ID3v2_Tag *id3v2_tag;
    size_t id3v2_tag_size;
    byte_t *file_content;
    size_t file_content_size;
} Tag_Manip_Info;

#endif
