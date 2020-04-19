/*
ps1-4git - A git-aware section for your PS1 prompt
Copyright (c) 2019, Yassen Damyanov

This program is free software: you can redistribute it and/or modify
it under the terms of the MIT License.
You should have received a copy of the MIT License along with this program.
If not, see <https://opensource.org/licenses/MIT>.
*/

/**
 * Unit tests for the ps1-4git module.
 *
 * Helpful URLs:
 *   https://stackoverflow.com/questions/38183974/cunit-assertion-assertion-void-0-f-pcursuite-failed
 *   http://cunit.sourceforge.net/doc/writing_tests.html
 *
 * See README.md on how to build and run the tests.
 *
 *   TODO:
 *    - Cover parse_input()
 *    - Provide end-to-end tests that send the binary different git-status outputs (python-based?)
 */

#include <stdlib.h>

#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#include "../ps1-4git.h"


void test_rindexof_expect_7(void) {
    //               0000000000111
    //               0123456789012
    char* somestr = "one two three";
    CU_ASSERT(7 == rindexof(' ', somestr, -1));
    CU_ASSERT(7 == rindexof(' ', somestr, 12));
    CU_ASSERT(7 == rindexof(' ', somestr, 10));
    CU_ASSERT(7 == rindexof(' ', somestr, 7));
}


void test_rindexof_expect_3(void) {
    //               0000000000111
    //               0123456789012
    char* somestr = "one two three";
    CU_ASSERT(3 == rindexof(' ', somestr, 6));
    CU_ASSERT(3 == rindexof(' ', somestr, 4));
    CU_ASSERT(3 == rindexof(' ', somestr, 3));
}


void test_rindexof_crawling_to_the_left(void) {
    //            0000000000111111111122222222223333333333444444444455
    //            0123456789012345678901234567890123456789012345678901
    char* line = "Your branch is ahead of 'origin/master' by 1 commit.";
    int ahead_right = rindexof(' ', line, -1);
    int ahead_left = rindexof(' ', line, ahead_right -1);
    int aheadof_count = atoi(&line[ahead_left + 1]);
    CU_ASSERT(44 == ahead_right);
    CU_ASSERT(42 == ahead_left);
    CU_ASSERT(1 == aheadof_count);
}


void test_startswith_true(void) {
    CU_ASSERT_TRUE(startswith("hello world", "hello"));
    CU_ASSERT_TRUE(startswith("", ""));
    CU_ASSERT_TRUE(startswith("anything", ""));
}


void test_startswith_false(void) {
    CU_ASSERT_FALSE(startswith("hello world", "the"));
    CU_ASSERT_FALSE(startswith("", "a"));
}


void test_isoneof_true(void) {
    CU_ASSERT_TRUE(isoneof("-s", "--sample-switch", "-s"));
    CU_ASSERT_TRUE(isoneof("-s", "--sample-switch", "--sample-switch"));
}


void test_isoneof_false(void) {
    CU_ASSERT_FALSE(isoneof("-s", "--sample-switch", "--sample-"));
    CU_ASSERT_FALSE(isoneof("-s", "--sample-switch", "--sample-switc"));
    CU_ASSERT_FALSE(isoneof("-s", "--sample-switch", ""));
}


int run_test_suites(void) {

    CU_initialize_registry();
    CU_pSuite suite = CU_add_suite("ps1-4git-test", 0, 0);

    CU_add_test(suite, "test_rindexof_expect_7", test_rindexof_expect_7);
    CU_add_test(suite, "test_rindexof_expect_3", test_rindexof_expect_3);
    CU_add_test(suite, "test_rindexof_crawling_to_the_left", test_rindexof_crawling_to_the_left);
    CU_add_test(suite, "test_startswith_true", test_startswith_true);
    CU_add_test(suite, "test_startswith_false", test_startswith_false);
    CU_add_test(suite, "test_isoneof_true", test_isoneof_true);
    CU_add_test(suite, "test_isoneof_false", test_isoneof_false);

    CU_basic_set_mode(CU_BRM_NORMAL); // CU_BRM_NORMAL, CU_BRM_SILENT, CU_BRM_VERBOSE
    CU_basic_run_tests();
    unsigned int tests_failed = CU_get_number_of_tests_failed();
    CU_cleanup_registry();

    int rc = tests_failed? 5: 0;
    return rc;
}


int main(void) {
    return run_test_suites();
}
