#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "message.h"
#include "utility.h"

void print_error_message(const char *const err_message) {
    assert(err_message != NULL);
    fputs(err_message, stderr);
}

void print_message(const char *const message) {
    assert(message != NULL);
    fputs(message, stdout);
}

void print_usage_format() {
    print_message("Usage:    mp3tag -[tTaAycghv] \"value\" filename\n");
    print_message("          mp3tag -t \"new title\" someMusic.mp3\n");
}

void print_options_menu() {
    print_usage_format();
    print_message("Options:\n");
    print_message("          -t\tModifies a Title tag.\n");
    print_message("          -T\tModifies a Track tag.\n");
    print_message("          -a\tModifies a Artist tag.\n");
    print_message("          -A\tModifies a Album tag.\n");
    print_message("          -y\tModifies a Year tag.\n");
    print_message("          -c\tModifies a Comment tag.\n");
    print_message("          -g\tModifies a Genre tag.\n");
    print_message("          -h\tDisplay this help info.\n");
    print_message("          -v\tView mp3 audio tag details.\n");
}

