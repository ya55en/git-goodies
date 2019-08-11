# Makefile for the git-ps1 module

CC=gcc
CCFLAGS=-O2 -march=native
LFLAGS=

CFILES=git-ps1.c

all:	git-ps1

git-ps1:	$(CFILES)
	$(CC)  $(CCFLAGS) $(CFILES) $(LFLAGS) -o git-ps1

git-ps1_prof:	$(CFILES)
	$(CC) -pg -ggdb $(CCFLAGS) $(CFILES) $(LFLAGS) -o git-ps1_prof

git-ps1_debug:	$(CFILES)
	$(CC) -g -ggdb $(CCFLAGS) $(CPPFILES) $(LFLAGS) -o git-ps1_debug

clean:
	rm -f git-ps1 git-ps1_prof git-ps1_debug

install:	git-ps1
	install -s -m 755 -o root -g root ./git-ps1 /usr/local/bin/
	grep -q git-ps1-MARKER ~/.bashrc || git-ps1 --4bashrc >> ~/.bashrc

uninstall:
	rm -f /usr/local/bin/git-ps1
	sed -i -n '/git-ps1-MARKER/!p' ~/.bashrc
	@echo "\n!! Restart shells where 'git-ps1: command not found' pops up."

.PHONY: all clean
