/** Header file for ps1-4git.c */

#include <stdbool.h>

#define VERSION "0.2.1"

int indexof(const char, const char*, int);
int rindexof(const char, const char*, int);
bool startswith(const char*, const char*);
int parse_input(bool);
bool isoneof(const char*, const char*, const char*);
int doit(int, char**);
