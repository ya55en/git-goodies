# ps1-4git - A git-aware section for your PS1 prompt
# Copyright (c) 2019, Yassen Damyanov
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the MIT License.
# You should have received a copy of the MIT License along with this program.
# If not, see <https://opensource.org/licenses/MIT>.

# Makefile for the ps1-4git module

CC=gcc
CCFLAGS=-O2 -march=native
WFLAGS=-Wall -Wfloat-equal -Wextra
LFLAGS=

CFILES=ps1-4git.h ps1-4git.c ps1-4git-main.c main.c
CFILES_4TESTS=ps1-4git.c ps1-4git-main.c tests/test-ps1-4git.c


all:	ps1-4git

ps1-4git:	$(CFILES)
	$(CC)  $(CCFLAGS) $(CFILES) $(LFLAGS) -o ps1-4git

ps1-4git-prof:	$(CFILES)
	$(CC) -pg -ggdb $(CCFLAGS) $(CFILES) $(LFLAGS) -o ps1-4git-prof

ps1-4git-debug:	$(CFILES)
	$(CC) -g -ggdb $(CCFLAGS) $(CFILES) $(LFLAGS) $(WFLAGS) -Wpedantic -o ps1-4git-debug

clean:
	rm -f ps1-4git ps1-4git-prof ps1-4git-debug tests/test-ps1-4git

test:   $(CFILES_4TESTS)
	$(CC)  $(CCFLAGS) $(CFILES_4TESTS) $(WFLAGS) $(LFLAGS) -lcunit -o tests/test-ps1-4git
	./tests/run-test.sh


SUDO_HOME = $(shell ./home-root.sh)/$(SUDO_USER)

install:	ps1-4git
	install -s -m 755 -o root -g root ./ps1-4git /usr/local/bin/
	grep -q ps1-4git-MARKER $(SUDO_HOME)/.bashrc || ps1-4git --4bashrc >> $(SUDO_HOME)/.bashrc
	@echo "!! Now do: source ~/.bashrc"

uninstall:  clean
	rm -f /usr/local/bin/ps1-4git
	sed -i -n '/ps1-4git-MARKER/!p' ~/.bashrc
	@echo "\n!! Restart shells where 'ps1-4git: command not found' pops up with:  source ~/.bashrc"

.PHONY: all clean
