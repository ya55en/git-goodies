/*
ps1-4git - A git-aware section for your PS1 prompt
Copyright (c) 2019, Yassen Damyanov

This program is free software: you can redistribute it and/or modify
it under the terms of the MIT License.
You should have received a copy of the MIT License along with this program.
If not, see <https://opensource.org/licenses/MIT>.
*/

/**
 * A main definition extracted in a separate .c file to avoid
 * redefinition conflict with test-ps1-4git.c's main().
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ps1-4git.h"


#define PS1_SETUP_STRING "PS1=\"$PS1\\$(git status 2>&1 | ps1-4git)\"  # ps1-4git-MARKER: Do NOT remove marker\n"
#define MSG_WRONG_ARGS "Wrong arguments. Try 'ps1-4git -h'\n"

const char* MSG_USAGE =
    "ps1-4git - git-aware section for your PS1 prompt\n"
    "https://github.com/yassen-itlabs/git-goodies\n"
    "\n"
    "Usage:\n"
    "\n"
    "  ps1-4git -h|--help          Print this help screen.\n"
    "  ps1-4git -v|--version       Show program version.\n"
    "\n"
    "  git status 2>&1 | ps1-4git [-n|--no-unicode]\n"
    "                                Parse 'git status' output and print informative\n"
    "                                prompt ending string.\n"
    "\n"
    "                             -n|--no-unicode switch causes the output to be\n"
    "                                ascii only (no fancy arrows or bullets)\n"
    "\n"
    "                                Put this in $() at the end of your PS1 like:\n"
    "                                  PS1=\"$PS1\\$(git status 2>&1 | ps1-4git)\"\n"
    "\n"
    "  ps1-4git --4bashrc            Print a PS1 definition like the one above\n"
    "                                suitable for inclusion in .bashrc.\n"
;


/**
 * Return true iff arg is equal to either short_sw or long_sw.
 */
bool isoneof(const char* short_sw, const char* long_sw, const char* arg) {
    return strcmp(short_sw, arg) * strcmp(long_sw, arg) == 0;
}


/**
 * Main routine - handle command line args and call appropriate sub-functions.
 */
int parse_args(int argc, char** argv) {
    if (argc > 2) {
        fputs(MSG_WRONG_ARGS, stdout);
        return 2;
    }
    if (1 == argc) {
        return parse_input(false);  // no_unicode=false
    }
    // else: if (argc == 2):
    if (isoneof("-h", "--help", argv[1])) {
        fputs(MSG_USAGE, stdout);
        return 0;
    }
    if (isoneof("-v", "--version", argv[1])) {
        printf("ps1-4git v.%s\n", VERSION);
        return 0;
    }
    if (strcmp("--4bashrc", argv[1]) == 0) {
        fputs(PS1_SETUP_STRING, stdout);
        return 0;
    }
    if (isoneof("-n", "--no-unicode", argv[1])) {
        return parse_input(true);  // no_unicode=true

    } else {
        fputs(MSG_WRONG_ARGS, stdout);
        return 0;
    }
}
