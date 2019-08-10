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

PS1='\[\e]0;\u@\h: \w\a\]${debian_chroot:+($debian_chroot)}\[\033[01;32m\]\u@\h\[\033[00m\]:\[\033[01;34m\]\w\[\033[00m\]\$$(test -d ./.git && git status | git-ps1) '

 */

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define EOL 10
#define SPC 32
#define NUL '\0'

#define PATH_MAX 255
#define LINE_MAX 1024
#define BRANCH_MAX 255

#define COL_RED "\e[31m"
#define COL_GRN "\e[32m"
#define COL_YEL "\e[33m"
#define COL_OFF "\e[0m"


bool startswith(const char *a, const char *b) {
   return (strncmp(a, b, strlen(b)) == 0);
}


int main(void) {
    char line[LINE_MAX];
    char thebranch[BRANCH_MAX];

    bool to_be_committed = false;
    bool not_staged = false;
    bool untracked = false;

    while(fgets(line, sizeof(line), stdin) != NULL) {  // reads newline too
        not_staged = not_staged || startswith(line, "Changes not staged for commit:");
        to_be_committed = to_be_committed || startswith(line, "Changes to be committed:");
        untracked = untracked || startswith(line, "Untracked files:");

        if (startswith(line, "On branch ")) {
            // overwrite line EOL with NUL to get rid of it
            char* p = strchr(line, EOL);
            line[(int)(p - line)] = NUL;  // (p - line) is the index of the EOL char
            // get the last word as the branch name
            strcpy(thebranch, line + sizeof("On branch ") - 1);
        }
    }
    fputs(" ", stdout);
    if (to_be_committed)  fputs(COL_GRN, stdout);
    else if (not_staged)  fputs(COL_RED, stdout);
    else if (untracked)  fputs(COL_YEL, stdout);
    printf("(br:%s)", thebranch);
    if (not_staged || to_be_committed || untracked) {
        fputs(COL_OFF, stdout);
    }

    return 0;
}
