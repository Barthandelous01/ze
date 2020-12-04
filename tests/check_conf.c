#include <check.h>
#include <stdlib.h>
#include <stdio.h>

#include "../src/conf.h"
#include "../src/io.h"
#include "../src/error.h"

/**
 * test_conf_no_file() - test nonexistent config
 *
 * test_conf_no_file() tests for proper behavior if the config file
 * specified does not exist.
 */
START_TEST (test_conf_no_file)
{
	char y[2*PATH_BUFSIZE];
	home_prefix("/ZZZXZZconfig.conf", y);
	remove(y);

	config x;
	parse_config("/ZZZXZZconfig.conf", &x);

	ck_assert(x.head == NULL);

	char z[CONF_KEY_SIZE];
	int res2 = get_config(&x, "TESTING", z);

	ck_assert(res2 == -EVAL);

	close_config(&x);
}
END_TEST

/**
 * test_conf_empty_file() - test empty config
 *
 * test_conf_empty_file() tests for the correct behavior if the
 * specified config file exists, but is empty.
 */
START_TEST (test_conf_empty_file)
{
	char y[2*PATH_BUFSIZE];
	home_prefix("/ZZZXZZconfig.conf", y);
	remove(y);

	FILE *fd = fopen(y, "w");
	if(fd == NULL)
		ck_abort_msg("Could not create file %s after deleting\n", y);

	if(fclose(fd) != 0)
		ck_abort_msg("Could not close file %s\n", y);

	config x;
	parse_config("/ZZZXZZconfig.conf", &x);

	char z[CONF_KEY_SIZE];
	int res2 = get_config(&x, "TESTING", z);

	ck_assert(res2 == -EVAL);

	close_config(&x);
}

/**
 * conf_suite() - generate the test suite for id
 *
 * conf_suite() is a boilerplate function that ties together the above
 * tests into a single unit.
 */
Suite *conf_suite(void)
{
	/* Boilerplate */
	Suite *s;
	TCase *tc_core;

	s = suite_create("ID");
	tc_core = tcase_create("Core");

	/* Add each test to be run */
	tcase_add_test(tc_core, test_conf_no_file);
	tcase_add_test(tc_core, test_conf_empty_file);

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

	s = conf_suite();
	sr = srunner_create(s);

	srunner_run_all(sr, CK_VERBOSE);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
