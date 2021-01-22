#include <stdlib.h>
#include <stdio.h>

#include "../src/conf.h"
#include "../src/io.h"
#include "../src/error.h"

#include "unix_unit.h"

/**
 * test_conf_no_file() - test nonexistent config
 *
 * test_conf_no_file() tests for proper behavior if the config file
 * specified does not exist.
 */
uMAKE_TEST (test_conf_no_file)
{
	char y[2*PATH_BUFSIZE];
	home_prefix("/ZZZXZZconfig.conf", y);
	remove(y);

	config x;
	parse_config("/ZZZXZZconfig.conf", &x);

	uassert(x.head == NULL);

	char z[CONF_KEY_SIZE];
	int res2 = get_config(&x, "TESTING", z);

	uassert(res2 == -EVAL);

	close_config(&x);
}
uEND_TEST

/**
 * test_conf_empty_file() - test empty config
 *
 * test_conf_empty_file() tests for the correct behavior if the
 * specified config file exists, but is empty.
 */
uMAKE_TEST (test_conf_empty_file)
{
	char y[2*PATH_BUFSIZE];
	home_prefix("/ZZZXZZconfig.conf", y);
	remove(y);

	FILE *fd = fopen(y, "w");
	if(fd == NULL)
		ufail;

	if(fclose(fd) != 0)
		ufail;

	config x;
	parse_config("/ZZZXZZconfig.conf", &x);

	char z[CONF_KEY_SIZE];
	int res2 = get_config(&x, "TESTING", z);

	uassert(res2 == -EVAL);

	close_config(&x);

	remove(y);
}
uEND_TEST

/**
 * test_conf_garbage_file() - test garbage file
 *
 * Make sure that parse_config() doesn't implode when fed a
 * garbage file.
 */
uMAKE_TEST (test_conf_garbage_file)
{
	/* skip for now... maybe it should implode? */
	uskip;

	char y[2*PATH_BUFSIZE];
	home_prefix("/ZZZXZZconfig.conf", y);
	remove(y);

	FILE *fd = fopen(y, "w");
	if(fd == NULL)
		ufail;
	fprintf(fd, "sefah32983hfasdf&#R98hssef\njf3qh\n\tq3f9jsdf\n\nTESTING=HELLO");
	if(fclose(fd) != 0)
		ufail;

	config x;
	parse_config("/ZZZXZZconfig.conf", &x);

	char z[CONF_KEY_SIZE];
	int res2 = get_config(&x, "TESTING", z);

	uassert(res2 == -EVAL);

	close_config(&x);

	remove(y);
}
uEND_TEST

/**
 * test_conf_normal() - test on a normal config file
 *
 * Make sure that parse_config() does correctly parse a syntactically
 * correct config file.
 */
uMAKE_TEST(test_conf_normal)
{
	char y[2*PATH_BUFSIZE];
	home_prefix("/ZZZXZZconfig.conf", y);
	remove(y);

	FILE *fd = fopen(y, "w");
	if(fd == NULL)
		ufail;
	fprintf(fd, "TESTING=HELLO\nEMPTY=\n # HI=BYEm");
	if(fclose(fd) != 0)
		ufail;

	config x;
	parse_config("/ZZZXZZconfig.conf", &x);

	char z[CONF_KEY_SIZE];

	get_config(&x, "TESTING", z);
	uassert_str_eq(z, "HELLO");

	get_config(&x, "EMPTY", z);
	uassert_str_eq(z, "");

	int res = get_config(&x, "HI", z);
	uassert(res == -EVAL);

	close_config(&x);

	remove(y);
}
uEND_TEST

/**
 * conf_suite() - generate the test suite for id
 *
 * conf_suite() is a boilerplate function that ties together the above
 * tests into a single unit.
 */
uMAKE_SUITE(conf_suite)
{
	uadd_test(test_conf_no_file);
	uadd_test(test_conf_empty_file);
	uadd_test(test_conf_garbage_file);
	uadd_test(test_conf_normal);
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
	uadd_suite(conf_suite);
}
uEND_HARNESS
