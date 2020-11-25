#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>

#include <config.h>

#include "id.h"
#include "db.h"
#include "io.h"
#include "conf.h"
#include "error.h"

static void version()
{
	printf("%s", PACKAGE_STRING "\nCopyright (C) 2020 Barthandelous01\n\
License RBSD 3-Clause License.\n\
This is free software; you are free to change and redistribute it.\n\
There is NO WARRANTY, to the extent permitted by law.");
	exit(0);
}

static void top_level_error(char *format, int code)
{
	fprintf(stderr, format, code);
	exit(code);
}

int main (int argc, char **argv)
{
	int res;

	ensure_directories();

	config cfg;
	cfg.head = NULL;
	if((res = parse_config(CONFIG_FILE, &cfg)))
		top_level_error(ERRMAIN"parse_config(): %d\n", res);

	if ((res = init_id(CONFIG_ID)))
		top_level_error(ERRMAIN"init_id(): %d\n", res);

	db_context db;
	if((res = init_db(&db)))
		top_level_error(ERRMAIN"init_db(): %d\n", res);

	static int quiet = 0;
	static struct option longopts[] = {
		{"version",    no_argument,       NULL, 'V'},
		{"help",       no_argument,       NULL, 'h'},
		{"new",        no_argument,       NULL, 'n'},
		{"edit",       required_argument, NULL, 'e'},
		{"remove",     required_argument, NULL, 'r'}
	};
	int ch = 0;
	while ((ch = getopt_long(argc, argv, "Vhne:r:", longopts, NULL)) != -1)
	{
		quiet = 1;
		switch(ch) {
		case 'V':
			version();
			break;
		}
	}

	if ((quiet == 0) && (argc >= 2))
		printf("Got single arg: %s\n", argv[1]);
	else
		printf("Do default thing\n");

	if((res = close_id()))
		top_level_error(ERRMAIN"close_id(): %d\n", res);

	if((res = close_config(&cfg)))
		top_level_error(ERRMAIN"close_config(): %d\n", res);

	close_db(&db);

	return EXIT_SUCCESS;
}
