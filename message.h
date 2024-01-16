#ifndef MESSAGE_H
#define MESSAGE_H

#include "types.h"

#define STR_ERR_MEM_ALLOC_FAIL 			"Error: Memory allocation failed.\n"

#define ANSI_COLOR_RED     			"\x1b[31m"
#define ANSI_COLOR_GREEN   			"\x1b[32m"
#define ANSI_COLOR_YELLOW  			"\x1b[33m"
#define ANSI_COLOR_BLUE    			"\x1b[34m"
#define ANSI_COLOR_MAGENTA 			"\x1b[35m"
#define ANSI_COLOR_CYAN    			"\x1b[36m"
#define ANSI_COLOR_RESET   			"\x1b[0m"

#define STR_ERR_OPR_FAIL 			"Error: Operation Failed.\n"
#define STR_ERR_FILEIO_FAIL 			"Error: File I/O failed.\n"
#define STR_ERR_ABSENT_TAG			"Error: ID3 Tag not found.\n"
#define STR_ERR_TAG_VAL_FAIL 			"Error: ID3 Tag validation failed.\n"
#define STR_ERR_UNKNOWN_ID3V2			"Error: Unknown ID3v2 tag version.\n"
#define STR_ERR_TAG_READ_FAIL 			"Error: ID3 Tag read failed.\n"

#define STR_ERR_NO_ARG				"Error: No arguments given.\n"
#define STR_ERR_INVALID_ARG			"Error: Invalid Argument.\n"
#define STR_ERR_NO_FILE_INPUT			"Error: No file input given.\n"
#define STR_ERR_NO_VALUE_STR			"Error: No value string given.\n"
#define STR_ERR_ARG_VAL				"Error: Argument validation failed.\n"

#define STR_ERR_ID3v24_HEADER_FLAGS 		"Error: ID3v2.4 header flags invalid.\n"
#define STR_ERR_SYNCSAFE_INT			"Error: ID3v2 syncsafe integer invalid.\n"
#define STR_ERR_ID3v2_HEADER_READ_FAIL 		"Error: ID3v2 header read failed.\n"
#define STR_ERR_ID3v24_HEADER_VAL_FAIL 		"Error: ID3v2 header validation failed.\n"
#define STR_ERR_ID3v24_EHEADER_READ_FAIL	"Error: ID3v2.4 ext. header read failed.\n"
#define STR_ERR_ID3v24_EHEADER_VAL_FAIL		"Error: ID3v2.4 ext. header validation failed.\n"
#define STR_ERR_ID3v23_EHEADER_READ_FAIL	"Error: ID3v2.3 ext. header read failed.\n"
#define STR_ERR_ID3v23_EHEADER_VAL_FAIL		"Error: ID3v2.3 ext. header validation failed.\n"
#define STR_ERR_ID3v24_EHEADER_FLAGS 		"Error: ID3v2.4 ext. header flags invalid.\n"
#define STR_WAR_FRAME_ID_INVALID		ANSI_COLOR_YELLOW "Warning: " ANSI_COLOR_RESET "ID3v2.4 Frame Header ID invalid.\n"
#define STR_WAR_UNKNOWN_TEXT_ENC 		ANSI_COLOR_YELLOW "Warning: " ANSI_COLOR_RESET "Unknown text encoding.\n"
#define	STR_ERR_FRAME_HDR_VAL_FAIL		"Error: ID3v2 Frame Header validation failed.\n"
#define	STR_ERR_FRAME_READ_FAIL                 "Error: ID3v2 Frame read failed.\n"
#define STR_ERR_SYNCSAFE_CONV 			"Error: Unable to process the size into syncsafe integer.\n"

#define STR_ERR_TAG_COPY_FAIL 			"Error: ID3 Tag copy into memory failed.\n"
#define STR_ERR_HEADER_COPY_FAIL 		"Error: ID3 header copy into memory failed.\n"
#define STR_ERR_EXT_HEADER_COPY_FAIL 		"Error: ID3 extended header copy into memory failed.\n"
#define STR_ERR_FRAME_COPY_FAIL 		"Error: ID3 frame copy into memory failed.\n"
#define STR_ERR_MP3_COPY_FAIL 			"Error: MP3 data copy into memory failed.\n"
#define STR_ERR_NO_TAG_FOUND			"Error: ID3 tag for the edit request not found in the MP3 file.\n"
#define STR_ERR_FILE_RENAME_FAIL		"Error: File renaming failed.\n"
#define STR_ERR_FILE_REMOVE_FAIL		"Error: File deletion failed.\n"

#define STR_PIC_TYPE_00				"Other"
#define STR_PIC_TYPE_01				"File Icon"
#define STR_PIC_TYPE_02				"Other File Icon"
#define STR_PIC_TYPE_03				"Cover (Front)"
#define STR_PIC_TYPE_04				"Cover (Back)"
#define STR_PIC_TYPE_05				"Leaflet Page"
#define STR_PIC_TYPE_06				"Media"
#define STR_PIC_TYPE_07				"Lead artist/lead performer/soloist"
#define STR_PIC_TYPE_08				"Artist/performer"
#define STR_PIC_TYPE_09				"Conductor"
#define STR_PIC_TYPE_0A				"Band/Orchestra"
#define STR_PIC_TYPE_0B				"Composer"
#define STR_PIC_TYPE_0C				"Lyricist/text writer"
#define STR_PIC_TYPE_0D				"Recording Location"
#define STR_PIC_TYPE_0E				"During recording"
#define STR_PIC_TYPE_0F				"During performance"
#define STR_PIC_TYPE_10				"Movie/video screen capture"
#define STR_PIC_TYPE_11				"A bright coloured fish"
#define STR_PIC_TYPE_12				"Illustration"
#define STR_PIC_TYPE_13				"Band/artist logotype"
#define STR_PIC_TYPE_14				"Publisher/Studio logotype"

#define STR_UNKNOWN				"Unknown"

void print_error_message(const char *const err_message);

void print_message(const char *const message);

void print_usage_format();

void print_options_menu();

#endif
