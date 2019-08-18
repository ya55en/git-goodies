/**
 * Scan 'git status' output (read from stdin) to detect git branch
 * and status of workspace files.
 */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "ps1-4git.h"

#define EOL 10
#define SPC 32
#define NUL '\0'
#define COMMA ','

#define PATH_MAX 255
#define LINE_MAX 1024
#define BRANCH_MAX 255

#define COL_RED "\e[31m"
#define COL_GRN "\e[32m"
#define COL_YEL "\e[33m"
#define COL_OFF "\e[0m"

#define PS1_SETUP_STRING "PS1=\"$PS1\\$(git status 2>&1 | ps1-4git -n)\"  # ps1-4git-MARKER: Do NOT remove marker\n"
#define MSG_WRONG_ARGS "Wrong arguments. Try 'ps1-4git -h'\n"

const char* MSG_USAGE =
    "ps1-4git - git-aware section in the PS1 prompt\n"
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
 * Return index of given char's occurance (searching left to right)
 * in str if found, -1 otherwise.
 */
int indexof(const char ch, const char* str, int start) {
    int idx = start;
    while (str[idx] != NUL && str[idx] != ch) {
        idx ++;
    }
    return (NUL == str[idx])? -1 : idx;
}


/**
 * Return index of given char's occurance (searching right to left)
 * in str if found, -1 otherwise.
 */
int rindexof(const char ch, const char* str, int start) {
    int idx = (start > -1)? start : (int)strlen(str) - 1;
    while (idx >= 0 && str[idx] != ch) {
        idx --;
    }
    return idx;
}


/**
 * Return true if string a starts with string b, false otherwise.
 */
bool startswith(const char* a, const char* b) {
    return (strncmp(a, b, strlen(b)) == 0);
}


/**
 * Expect the output of 'git status' on stdin; scan it for certain patterns
 * and output appropriate combination of strings with data forming he last
 * part of a PS1 shell prompt. Return appropriate result code.
 * If no_unicode is true -- use ascii characters in place of symbols (arrows,
 * bullets).
 */
int parse_input(bool no_unicode) {

    char line[LINE_MAX];
    char thebranch[BRANCH_MAX];

    bool to_be_committed = false;
    bool not_staged = false;
    bool untracked = false;

    // bool uptodate = false;
    int aheadof_count = -1;
    int behind_count = -1;

    while (fgets(line, sizeof(line), stdin) != NULL) {  // reads newline too
        if (startswith(line, "fatal: ")) {
            return 128;  // return same rc as 'git status' when not in a git repo

        } else if (startswith(line, "On branch ")) {
            // overwrite line EOL with NUL to get rid of it
            char* p = strchr(line, EOL);
            line[(int)(p - line)] = NUL;  // (p - line) is the index of the EOL char
            // get the last word as the branch name
            strcpy(thebranch, line + sizeof("On branch ") - 1);

        } else if (startswith(line, "Your branch is ahead of ")) {
            int right = rindexof(SPC, line, -1);
            int left = rindexof(SPC, line, right -1);
            aheadof_count = atoi(&line[left + 1]);

        } else if (startswith(line, "Your branch is behind ")) {
            int comma = rindexof(COMMA, line, -1);
            int right = rindexof(SPC, line, comma -1);
            int left = rindexof(SPC, line, right -1);
            behind_count = atoi(&line[left + 1]);

        // } else if (startswith(line, "Your branch is up to date ")) {
        //    uptodate = true;

        } else {
            not_staged = not_staged || startswith(line, "Changes not staged for commit:");
            to_be_committed = to_be_committed || startswith(line, "Changes to be committed:");
            untracked = untracked || startswith(line, "Untracked files:");
        }
    }

    // Unicodes: middle: A537 (used)  big: 27A1+space, super small bulet 2022 or 2981)
    // useful unicode char map: https://unicode-table.com/en/#hangul-syllables

    char* bullet = no_unicode? "*" : "ꔷ";
    char* left_arrow = no_unicode? " <" : "⬅ ";
    char* right_arrow = no_unicode? " >" : "➡ ";

    printf("(");  // TODO: denote push with ⭡2 (2B61), pull with ⭣3 (2B63)
    if (to_be_committed)  fputs(COL_GRN, stdout);
    else if (not_staged)  fputs(COL_RED, stdout);
    else if (untracked)  fputs(COL_YEL, stdout);

    fputs(thebranch, stdout);

    if (not_staged || to_be_committed || untracked) {
        fputs(COL_OFF, stdout);
    }
    if (aheadof_count > -1) {
        printf("%s%s%d%s) ", COL_RED, right_arrow, aheadof_count, COL_OFF);  // 27A1 + spc
    } else if (behind_count > -1) {
        printf("%s%s%d%s) ", COL_YEL, left_arrow, behind_count, COL_OFF);  // 2b05 is <-
    } else {
        printf(" %s) ", bullet);
    }
    return 0;
}


/**
 * Return true iff arg is equal to either short_sw or long_sw.
 */
bool isoneof(const char* short_sw, const char* long_sw, const char* arg) {
    return strcmp(short_sw, arg) * strcmp(long_sw, arg) == 0;
}


/**
 * Main routine - handle command line args and call appropriate sub-functions.
 */
int doit(int argc, char** argv) {
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
