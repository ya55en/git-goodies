/*
ps1-4git - A git-aware section for your PS1 prompt
Copyright (c) 2019, Yassen Damyanov

This program is free software: you can redistribute it and/or modify
it under the terms of the MIT License.
You should have received a copy of the MIT License along with this program.
If not, see <https://opensource.org/licenses/MIT>.
*/

/**
 * Scan `git status` output (reading it from stdin) to detect git branch
 * and status of workspace files, then output a PS1-suited prompt section
 * showing the branch and it's status.  See also README.md.
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

#define COL_RED "\001\033[31m\002"
#define COL_GRN "\001\033[32m\002"
#define COL_YEL "\001\033[33m\002"
#define COL_OFF "\001\033[0m\002"


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
