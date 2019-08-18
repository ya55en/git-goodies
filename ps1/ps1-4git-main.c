/**
 * A main definition extracted in a separate .c file to avoid
 * redefinition conflict with test-ps1-4git.c's main().
 */

#include "ps1-4git.h"

int main(int argc, char **argv) {
    return doit(argc, argv);
}
