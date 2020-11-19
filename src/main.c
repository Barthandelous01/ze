#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <lmdb.h>

#include <config.h>

#include "id.h"
#include "error.h"

static void version()
{
	printf("%s", PACKAGE_STRING "\nCopyright (C) 2020 Barthandelous01\n\
License RBSD 3-Clause License.\n\
This is free software; you are free to change and redistribute it.\n\
There is NO WARRANTY, to the extent permitted by law.");
	exit(0);
}

int main (int argc, char **argv)
{

	int res = init_id(CONFIG_ID);
	if (res){
		fprintf(stderr, "An error occured in init_id: %d\n", res);
		exit(-EFILE);
	}

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

	if(close_id() != SUCCESS) {
		fprintf(stderr, "%s\n", "An error has occured");
	}

	return EXIT_SUCCESS;
}
