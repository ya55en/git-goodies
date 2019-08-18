/*
ps1-4git - A git-aware section for your PS1 prompt
Copyright (c) 2019, Yassen Damyanov

This program is free software: you can redistribute it and/or modify
it under the terms of the MIT License.
You should have received a copy of the MIT License along with this program.
If not, see <https://opensource.org/licenses/MIT>.
*/

/**
 * A main definition extracted in a separate .c file to avoid
 * redefinition conflict with test-ps1-4git.c's main().
 */

#include "ps1-4git.h"

int main(int argc, char **argv) {
    return doit(argc, argv);
}
