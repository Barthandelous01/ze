#include <check.h>
#include <stdlib.h>
#include <stdio.h>

#include "../src/conf.h"
#include "../src/error.h"

START_TEST (test_)
{
	;
}
END_TEST


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
	tcase_add_test(tc_core, test_);

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
