#include <stdlib.h>
#include <stdio.h>

#include "../src/id.h"
#include "../src/io.h"
#include "../src/error.h"

#include "../src/id.c"

#include "unix_unit.h"

/**
 * test_init_id() - a SIGSEV test for test_init_id()
 *
 * It's important to make sure that just running init_id()
 * "as intended" doesn't cause a SIGSEV or other form of
 * crash behavior.
 */
uMAKE_TEST (test_init_id)
{
	int res2 = init_id("/.ze/current.id");
	uassert(res2 == 0);
	close_id();
}
uEND_TEST

/**
 * test_get_id_no_file() - test on a non-existant file
 *
 * This makes sure that if the file init_id() is called on
 * doesn't exist that everything doesn't break.
 */
uMAKE_TEST (test_get_id_no_file)
{
	/* Setup: make sure file doesn't exist */
	char y[2*PATH_BUFSIZE];
	home_prefix("/ZZZZZZtest.id", y);
	remove(y);

	/* Make sure there are no initial failures */
	uassert(init_id("/ZZZZZZtest.id") == 0);

	char x[20];

	get_id(x);
	uassert_str_eq(x, "000001");

	get_id(x);
	uassert_str_eq(x, "000002");

	close_id();

	/* Teardown: don't leave random files in the user's home dir */
	remove(y);
}
uEND_TEST

/**
 * test_get_id_empty_file() - test on an empty file
 *
 * Ensure the correct behavior of init_id() when called with
 * an empty file.
 */
uMAKE_TEST (test_get_id_empty_file)
{
	/* Setup: make sure file is empty */
	char y[2*PATH_BUFSIZE];
	home_prefix("/ZZZZZZtest.id", y);
	remove(y);
	FILE *fd = fopen(y, "w+");
	if (fd == NULL)
		ufail;

	if(fclose(fd) != 0)
		ufail;

	/* Make sure there are no initial failures */
	uassert(init_id("/ZZZZZZtest.id") == 0);

	char x[20];

	get_id(x);
	uassert_str_eq(x, "000001");

	get_id(x);
	uassert_str_eq(x, "000002");

	close_id();

	/* Teardown: don't leave random files in the user's home dir */
	remove(y);
}
uEND_TEST

/**
 * test_get_id_garbage_file() - test on garbage file
 *
 * test_get_id_garbage_file() tests init_id()'s behavior when
 * called on a file with garbage in it.
 */
uMAKE_TEST (test_get_id_garbage_file)
{
	/* Setup: make sure file is empty */
	char y[2*PATH_BUFSIZE];
	home_prefix("/ZZZZZZtest.id", y);
	remove(y);
	FILE *fd = fopen(y, "w+");
	if (fd == NULL)
		ufail;
	fprintf(fd, "%s", "hfksdfi8h3qlks\n\n\n\to23fasdf\n");
	if(fclose(fd) != 0)
		ufail;

	/* Make sure there are no initial failures */
	uassert(init_id("/ZZZZZZtest.id") == 0);

	char x[20];

	get_id(x);
	uassert_str_eq(x, "000001");

	get_id(x);
	uassert_str_eq(x, "000002");

	close_id();

	/* Teardown: don't leave random files in the user's home dir */
	remove(y);
}
uEND_TEST

/**
 * id_suite() - generate the test suite for id
 *
 * id_suite() is a boilerplate function that ties together the above
 * tests into a single unit.
 */
uMAKE_SUITE(id_suite)
{
	uadd_test(test_init_id);
	uadd_test(test_get_id_empty_file);
	uadd_test(test_get_id_garbage_file);
	uadd_test(test_get_id_no_file);
}
uEND_SUITE

/**
 * main() - run the test case
 *
 * Each module will have a seperate C file that builds an autotools test
 * using check. main() looks basically identical in any of them.
 */
uTEST_HARNESS
{
	uadd_suite(id_suite);
}
uEND_HARNESS
