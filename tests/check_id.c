#include <check.h>
#include <stdlib.h>
#include "../src/id.h"

START_TEST (test_init_id)
{
	int res2 = init_id("/.ze/current.id");
	ck_assert(res2 == 0);
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

	srunner_run_all(sr, CK_NORMAL);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
