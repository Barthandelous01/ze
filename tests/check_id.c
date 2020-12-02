#include <check.h>
#include <stdlib.h>
#include <stdio.h>

#include "../src/id.h"
#include "../src/io.h"
#include "../src/error.h"

/**
 * test_init_id() - a SIGSEV test for test_init_id()
 *
 * It's important to make sure that just running init_id()
 * "as intended" doesn't cause a SIGSEV or other form of
 * crash behavior.
 */
START_TEST (test_init_id)
{
	int res2 = init_id("/.ze/current.id");
	ck_assert(res2 == 0);
	close_id();
}
END_TEST

/**
 * test_get_id_no_file() - test on a non-existant file
 *
 * This makes sure that if the file init_id() is called on
 * doesn't exist that everything doesn't break.
 */
START_TEST (test_get_id_no_file)
{
	/* Setup: make sure file doesn't exist */
	char y[2*PATH_BUFSIZE];
	home_prefix("/ZZZZZZtest.id", y);
	remove(y);

	/* Make sure there are no initial failures */
	ck_assert(init_id("/ZZZZZZtest.id") == 0);

	char x[20];

	get_id(x);
	ck_assert_pstr_eq(x, "000001");

	get_id(x);
	ck_assert_pstr_eq(x, "000002");

	close_id();

	/* Teardown: don't leave random files in the user's home dir */
	remove(y);
}
END_TEST

/**
 * test_get_id_empty_file() - test on an empty file
 *
 * Ensure the correct behavior of init_id() when called with
 * an empty file.
 */
START_TEST (test_get_id_empty_file)
{
	/* Setup: make sure file is empty */
	char y[2*PATH_BUFSIZE];
	home_prefix("/ZZZZZZtest.id", y);
	remove(y);
	FILE *fd = fopen(y, "w+");
	if (fd == NULL)
		ck_abort_msg("Could not create file %s after deleting\n", y);

	if(fclose(fd) != 0)
		ck_abort_msg("Could not close file %s\n", y);

	/* Make sure there are no initial failures */
	ck_assert(init_id("/ZZZZZZtest.id") == 0);

	char x[20];

	get_id(x);
	ck_assert_pstr_eq(x, "000001");

	get_id(x);
	ck_assert_pstr_eq(x, "000002");

	close_id();

	/* Teardown: don't leave random files in the user's home dir */
	remove(y);
}
END_TEST

/**
 * test_get_id_garbage_file() - test on garbage file
 *
 * test_get_id_garbage_file() tests init_id()'s behavior when
 * called on a file with garbage in it.
 */
START_TEST (test_get_id_garbage_file)
{
	/* Setup: make sure file is empty */
	char y[2*PATH_BUFSIZE];
	home_prefix("/ZZZZZZtest.id", y);
	remove(y);
	FILE *fd = fopen(y, "w+");
	if (fd == NULL)
		ck_abort_msg("Could not create file %s after deleting\n", y);
	fprintf(fd, "%s", "hfksdfi8h3qlks\n\n\n\to23fasdf\n");
	if(fclose(fd) != 0)
		ck_abort_msg("Could not close file %s\n", y);

	/* Make sure there are no initial failures */
	ck_assert(init_id("/ZZZZZZtest.id") == 0);

	char x[20];

	get_id(x);
	ck_assert_pstr_eq(x, "000001");

	get_id(x);
	ck_assert_pstr_eq(x, "000002");

	close_id();

	/* Teardown: don't leave random files in the user's home dir */
	remove(y);
}
END_TEST

/**
 * id_suite() - generate the test suite for id
 *
 * id_suite() is a boilerplate function that ties together the above
 * tests into a single unit.
 */
Suite * id_suite(void)
{
	/* Boilerplate */
	Suite *s;
	TCase *tc_core;

	s = suite_create("ID");
	tc_core = tcase_create("Core");

	/* Add each test to be run */
	tcase_add_test(tc_core, test_init_id);
	tcase_add_test(tc_core, test_get_id_no_file);
	tcase_add_test(tc_core, test_get_id_empty_file);
	tcase_add_test(tc_core, test_get_id_garbage_file);

	suite_add_tcase(s, tc_core);
	return s;
}

/**
 * main() - run the test case
 *
 * Each module will have a seperate C file that builds an autotools test
 * using check. main() looks basically identical in any of them.
 */
int main (void)
{
	int number_failed;
	Suite *s;
	SRunner *sr;

	s = id_suite();
	sr = srunner_create(s);

	srunner_run_all(sr, CK_VERBOSE);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
