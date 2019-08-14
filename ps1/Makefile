# Makefile for the ps1-4git module

CC=gcc
CCFLAGS=-O2 -march=native
LFLAGS=

CFILES=ps1-4git.c


all:	ps1-4git

ps1-4git:	$(CFILES)
	$(CC)  $(CCFLAGS) $(CFILES) $(LFLAGS) -o ps1-4git

ps1-4git_prof:	$(CFILES)
	$(CC) -pg -ggdb $(CCFLAGS) $(CFILES) $(LFLAGS) -o ps1-4git_prof

ps1-4git_debug:	$(CFILES)
	$(CC) -g -ggdb $(CCFLAGS) $(CPPFILES) $(LFLAGS) -o ps1-4git_debug

clean:
	rm -f ps1-4git ps1-4git_prof ps1-4git_debug


HOME_ROOT = $(shell ./home-root.sh)
SUDO_HOME = $(HOME_ROOT)/$(SUDO_USER)

install:	ps1-4git
	install -s -m 755 -o root -g root ./ps1-4git /usr/local/bin/
	grep -q ps1-4git-MARKER $(SUDO_HOME)/.bashrc || ps1-4git --4bashrc >> $(SUDO_HOME)/.bashrc

uninstall:
	rm -f /usr/local/bin/ps1-4git
	sed -i -n '/ps1-4git-MARKER/!p' ~/.bashrc
	@echo "\n!! Restart shells where 'ps1-4git: command not found' pops up."

.PHONY: all clean
