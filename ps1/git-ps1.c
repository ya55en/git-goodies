/**
 * Scan 'git status' output (read from stdin) to detect git branch
 * and status of workspace files.
 * Sample 'git status' output:

----cut here--------------
  On branch bionic
  Your branch is ahead of 'origin/bionic' by 1 commit.
  (use "git push" to publish your local commits)

  Changes not staged for commit:
  (use "git add/rm <file>..." to update what will be committed)
  (use "git checkout -- <file>..." to discard changes in working directory)

      modified:   ansible.cfg
      modified:   hosts

  Untracked files:
  (use "git add <file>..." to include in what will be committed)

      dat-12-plays/
      roles/
      scripts/
      plays/win-baseline.yml

  no changes added to commit (use "git add" and/or "git commit -a")
----cut here--------------


To make use of this module, first compile and install:

$ git clone --depth=1 https://gitlab.itlabs.bg/open-source/git-goodies.git
$ cd git-goodies/ps1
$ sudo make install

Then add $(test -d ./.git && git status | git-ps1) to your PS1 environment variable,
similar to what is shown below:

PS1='\[\e]0;\u@\h: \w\a\]${debian_chroot:+($debian_chroot)}\[\033[01;32m\]\u@\h\[\033[00m\]:\[\033[01;34m\]\w\[\033[00m\]\$$(git status 2>&1 | git-ps1) '


[1] Example for git status after commit, BEFORE push
----cut here--------------
yassen@ubuntu:~/Work/git-goodies$ (br:master) git status
On branch master
Your branch is ahead of 'origin/master' by 1 commit.
  (use "git push" to publish your local commits)

nothing to commit, working tree clean
----cut here--------------

Error messages when not a git repo:
  "fatal: not a git repository (or any of the parent directories): .git"
  "fatal: this operation must be run in a work tree"

 */

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define VERSION "0.1.2"

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

#define PS1_SETUP_STRING "PS1=\"$PS1\\$(git status 2>&1 | git-ps1)\"  # git-ps1-MARKER: Do NOT remove marker\n"
#define MSG_WRONG_ARGS "Wrong arguments. Try 'ps1-4git -h'\n"

const char* MSG_USAGE =
    "ps1-4git - fancy git dash(board) in the prompt\n"
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
int indexof(const char ch, const char *str, int start) {
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
int rindexof(const char ch, const char *str, int start) {
    int idx = (start > -1)? start : strlen(str) - 1;
    while (idx >= 0 && str[idx] != ch) {
        idx --;
    }
    return idx;
}


/**
 * Return true if string a starts with string b, false otherwise.
 */
bool startswith(const char *a, const char *b) {
    return (strncmp(a, b, strlen(b)) == 0);
}


/**
 * Expect the output of 'git status' on stdin; scan it for certain patterns
 * and output appropriate combination of strings with data forming he last
 * part of a PS1 shell prompt. Return appropriate result code.
 * If no_unicode is true -- replace any unicode  symbols (arrows, bullet)
 * with ascii characters.
 */
int parse_input(bool no_unicode) {

    char line[LINE_MAX];
    char thebranch[BRANCH_MAX];

    bool to_be_committed = false;
    bool not_staged = false;
    bool untracked = false;

    bool uptodate = false;
    int aheadof_origin = 0;
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

        } else if (startswith(line, "Your branch is up to date ")) {
            uptodate = true;

        } else {
            not_staged = not_staged || startswith(line, "Changes not staged for commit:");
            to_be_committed = to_be_committed || startswith(line, "Changes to be committed:");
            untracked = untracked || startswith(line, "Untracked files:");
        }
    }

    // Unicodes: middle: A537 (used)  big: 27A1+space, super small bulet 2022 or 2981)
    // useful unicode char map: https://unicode-table.com/en/#hangul-syllables

    char* bullet = no_unicode? "*" : "ꔷ";
    char* left_arrow = no_unicode? "<-" : "⬅ ";
    char* right_arrow = no_unicode? "->" : "➡ ";

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


int main(int argc, char **argv) {
    if (argc > 2) {
        fputs(MSG_WRONG_ARGS, stdout);
        return 2;
    }
    if (argc == 2 && (strcmp("-h", argv[1]) * strcmp("--help", argv[1]) == 0)) {
        fputs(MSG_USAGE, stdout);
        return 0;
    }
    if (argc == 2 && (strcmp("-v", argv[1]) * strcmp("--version", argv[1]) == 0)) {
        printf("ps1-4git v.%s\n", VERSION);
        return 0;
    }
    if (argc == 2 && strcmp("--4bashrc", argv[1]) == 0) {
        fputs(PS1_SETUP_STRING, stdout);
        return 0;
    }
    if (argc == 2 && (strcmp("-n", argv[1]) * strcmp("--no-unicode", argv[1]) == 0)) {
        return parse_input(true);  // no_unicode=true
    }
    if (argc == 2) {
        fputs(MSG_WRONG_ARGS, stdout);
        return 0;
    }
    // else:
    return parse_input(false);  // no_unicode=false
}
