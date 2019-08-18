/*
ps1-4git - A git-aware section for your PS1 prompt
Copyright (c) 2019, Yassen Damyanov

This program is free software: you can redistribute it and/or modify
it under the terms of the MIT License.
You should have received a copy of the MIT License along with this program.
If not, see <https://opensource.org/licenses/MIT>.
*/

/** Header file for ps1-4git.c */

#include <stdbool.h>

#define VERSION "0.2.1"

int indexof(const char, const char*, int);
int rindexof(const char, const char*, int);
bool startswith(const char*, const char*);
int parse_input(bool);
bool isoneof(const char*, const char*, const char*);
int doit(int, char**);
