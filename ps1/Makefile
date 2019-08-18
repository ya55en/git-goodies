# Makefile for the ps1-4git module

CC=gcc
CCFLAGS=-O2 -march=native
WFLAGS=-Wall -Wfloat-equal -Wextra
LFLAGS=

CFILES=ps1-4git.c ps1-4git-main.c
CFILES4TESTS=ps1-4git.c tests/test-ps1-4git.c


all:	ps1-4git

ps1-4git:	$(CFILES)
	$(CC)  $(CCFLAGS) $(CFILES) $(LFLAGS) -o ps1-4git

ps1-4git-prof:	$(CFILES)
	$(CC) -pg -ggdb $(CCFLAGS) $(CFILES) $(LFLAGS) -o ps1-4git-prof

ps1-4git-debug:	$(CFILES)
	$(CC) -g -ggdb $(CCFLAGS) $(CFILES) $(LFLAGS) $(WFLAGS) -Wpedantic -o ps1-4git-debug

clean:
	rm -f ps1-4git ps1-4git-prof ps1-4git-debug

test:   $(CFILES4TESTS)
	$(CC)  $(CCFLAGS) $(CFILES4TESTS) $(WFLAGS) $(LFLAGS) -lcunit -o tests/test-ps1-4git
	./tests/run-test.sh


SUDO_HOME = $(shell ./home-root.sh)/$(SUDO_USER)

install:	ps1-4git
	install -s -m 755 -o root -g root ./ps1-4git /usr/local/bin/
	grep -q ps1-4git-MARKER $(SUDO_HOME)/.bashrc || ps1-4git --4bashrc >> $(SUDO_HOME)/.bashrc

uninstall:
	rm -f /usr/local/bin/ps1-4git
	sed -i -n '/ps1-4git-MARKER/!p' ~/.bashrc
	@echo "\n!! Restart shells where 'ps1-4git: command not found' pops up."

.PHONY: all clean
