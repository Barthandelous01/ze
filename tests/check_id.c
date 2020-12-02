#include <check.h>
#include <stdlib.h>
#include <stdio.h>

#include "../src/id.h"
#include "../src/io.h"
#include "../src/error.h"

/*
 * Basically just a sanity/SIGSEV test for
 * init_id()
 */
START_TEST (test_init_id)
{
	int res2 = init_id("/.ze/current.id");
	ck_assert(res2 == 0);
	close_id();
}
END_TEST

/*
 * When doing stuff with config files, we often need
 * to reset a file to empty. This does that.
 */
void reset_file(char *filepath)
{
	char x[2*PATH_BUFSIZE];
	home_prefix(filepath, x);
	remove(x);

	FILE *fd = fopen(x, "w+");
	fclose(fd);
}

/*
 * Do an actual test of  */
START_TEST (test_get_id)
{
	reset_file("/test.id");
	ck_assert(init_id("/test.id") == 0);

	char x[20];

	get_id(x);
	ck_assert_pstr_eq(x, "000001");

	get_id(x);
	ck_assert_pstr_eq(x, "000002");

	close_id();
}
END_TEST

Suite * id_suite(void)
{
	Suite *s;
	TCase *tc_core;

	s = suite_create("ID");
	tc_core = tcase_create("Core");

	tcase_add_test(tc_core, test_init_id);
	tcase_add_test(tc_core, test_get_id);

	suite_add_tcase(s, tc_core);
	return s;
}

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
