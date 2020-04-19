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
#define COL_MGT "\001\033[95m\002"  // light magenta, indicating diverged status
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
    bool diverged = false;

    // bool uptodate = false;
    int aheadof_count = -1;
    int behind_count = -1;
    int diverged_local = -1;
    int diverged_remote = -1;

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

        } else if (startswith(line, "Your branch and ")) {
            // handle diverged status (history changed locally e.g. after 'commit --amend')
            if (fgets(line, sizeof(line), stdin) == NULL) {  // read the next line
                // should never be reached (but bail just in case)
                fputs("<second line is NULL while handling diverged state>", stderr);
            }
            int first = sizeof("and have ") - 1;
            diverged_local = atoi(&line[first]);  // diferent commits on local branch
            int second = indexof(SPC, line, first) + sizeof("and ");
            diverged_remote = atoi(&line[second]);  // diferent commits on local branch
            diverged = true;

        // } else if (startswith(line, "Your branch is up to date ")) {
        //    uptodate = true;

        } else {
            not_staged = not_staged || startswith(line, "Changes not staged for commit:");
            to_be_committed = to_be_committed || startswith(line, "Changes to be committed:");
            untracked = untracked || startswith(line, "Untracked files:");
        }
    }

    // Unicodes: middle: A537 (used)  big: 27A1+space, super small bulet 2022 or 2981)
    // useful unicode char map: https://unicode-table.com/en/#dingbats

    char* bullet = no_unicode? "*" : "ꔷ";
    char* left_arrow = no_unicode? " <" : "⬅ ";  // U+2B05
    char* right_arrow = no_unicode? " >" : "➡ ";  // U+27A1
    char* two_way = no_unicode? "|" : "⬍ ";  // U+2B0D (was 2718)

    printf("(");
    if (to_be_committed)  fputs(COL_GRN, stdout);
    else if (not_staged)  fputs(COL_RED, stdout);
    else if (untracked)  fputs(COL_YEL, stdout);
    // else if (diverged)  fputs(COL_MGT, stdout); // let's have it just off-color

    fputs(thebranch, stdout);
    if (not_staged || to_be_committed || untracked) {
        fputs(COL_OFF, stdout);
    }

    if (diverged) {
        printf("%s %d%s%s%s%d) ", COL_OFF, diverged_local, COL_RED, two_way, COL_OFF, diverged_remote);
        return 0;
    }

    // else
    if (aheadof_count > -1) {
        printf("%s%s%d%s) ", COL_RED, right_arrow, aheadof_count, COL_OFF);

    } else if (behind_count > -1) {
        printf("%s%s%d%s) ", COL_YEL, left_arrow, behind_count, COL_OFF);

    } else {
        printf(" %s) ", bullet);
    }

    return 0;
}
