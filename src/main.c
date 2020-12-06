#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>

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
There is NO WARRANTY, to the extent permitted by law.\n");
	exit(EXIT_SUCCESS);
}

static void help()
{
	printf("%s\n", PACKAGE_STRING "\nCopyright (C) 2020 Barthandelous01\n"
		"\n"
		"OPTIONS:\n"
		"\t-v, --version      Print version information\n"
		"\t-h, --help         Print this message and exit\n"
		"\t-n, --new          Create a new zettel\n"
		"\t-e, --edit [id]   Edit the zettel [id]\n"
		"\t-r, --remove [id] Remove the zettel [id] (DANGEROUS!)\n");
	exit(EXIT_SUCCESS);
}

static void top_level_error(char *format, int code)
{
	fprintf(stderr, format, code);
	exit(code);
}

/**
 * print_index() - print index zettel
 * @cfg: the config struct to use to check for default zettel
 * @DB: the database of zettel
 *
 * print_index() prints the index zettel, which defaults to
 * 000001, checking if a different index zettel has been set in
 * the user config file.
 */
static void print_index(config *cfg, db_context *DB)
{
	char path[2*PATH_BUFSIZE], val[CONF_KEY_SIZE];
	memset(path, '\0', 2*PATH_BUFSIZE);
	int res = 0;
	if ((res = get_config(cfg, "INDEX_NODE", val)) == 0) {
		if ((res = get_record(DB, val, path)) != SUCCESS)
			top_level_error(ERRZE"Zettel not found: %d\n", res);
		else
			print_zettel(cfg, path);
	} else {
		if ((res = get_record(DB, "000001", path)) != SUCCESS)
			top_level_error(ERRZE"Zettel not found: %d\n", res);
		else
			print_zettel(cfg, path);
	}
}

/**
 * get_zettel() - get the path of a zettel
 * @DB: the database handle
 * @id: the id to print
 * @path: the resultant path of the zettel
 *
 * get_zettel() does a lookup in the db of zettel and returns
 * the full pathname of
 */
static int get_zettel(db_context *DB, char *id, char *path)
{
	char temp[2*PATH_BUFSIZE];
	int res;

	if ((res = get_record(DB, id, temp)) != SUCCESS) {
		return -EVAL;
	} else {
		strncpy(path, temp, 2*PATH_BUFSIZE);
	}
	return SUCCESS;
}

/**
 * modify_zettel() - edit the zettel given by pathname
 * @cfg: config context
 * @pathname: the pathname to edit. HOME CANONICALIZED
 *
 * modify_zettel() is basically an interace-module frontend to
 * io's edit_zettel(). Bubbles all errors up directly.
 */
static int modify_zettel(config *cfg, char *pathname)
{
	;
}

/**
 * create_zettel() - add a zettel to the DB
 * @DB: the database context
 * @cfg: config context
 *
 * create_zettel() creates the next serial zettel and then offloads the work of
 * writing it to edit_zettel(). Bubbles most errors up to caller unchanged, or
 * SUCCESS on success.
 */
static int create_zettel(db_context *DB, config *cfg)
{
	int res;
	char tmp[2*PATH_BUFSIZE], id[20], tmp2[PATH_BUFSIZE];
	char conf_ext[CONF_KEY_SIZE];

	memset(tmp, '\0', 2*PATH_BUFSIZE);

	if ((res = get_id(id)) != SUCCESS)
			return res;

	strcpy(tmp, ZETTEL_DIR);
	printf("%s: ", "Please enter a title for the zettel, without spaces or"
		" path seperators");
	scanf("%s", tmp2);
	strcat(tmp, "/");
	strcat(tmp, tmp2);

	if ((res = get_config(cfg, "FILE_FORMAT", conf_ext)) != SUCCESS)
		strcat(tmp, DEF_EXT);
	else
		strcat(tmp, conf_ext);

	if ((res = add_record(DB, id, tmp)) != SUCCESS)
		return res;

	if ((res = edit_zettel(cfg, tmp)) != SUCCESS)
		return res;

	return SUCCESS;
}

int main (int argc, char **argv)
{
	int res, ch;
	config cfg;

	ensure_directories();

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

	while ((ch = getopt_long(argc, argv, "Vhne:r:", longopts, NULL)) != -1)
	{
		quiet = 1;
		switch(ch) {
		case 'V':
			version();
			break;
		case 'h':
			help();
			break;
		case 'n':
			if ((res = create_zettel(&db, &cfg)) != SUCCESS)
				top_level_error(ERRMAIN"Making zettel: %d\n", res);
			break;
		}
	}

	if (quiet == 0 && (argc >= 2)) {
		char zettel[2*PATH_BUFSIZE];
		for(int j = 1; j < argc; j++) {
			res = get_zettel(&db, argv[j], zettel);
			if(res != SUCCESS)
				top_level_error(ERRZE
						"Zettel not found: %d\n", res);

			res = print_zettel(&cfg, zettel);
			if(res != SUCCESS)
				top_level_error(ERRZE
						"Zettel not found: %d\n", res);
		}
	} else if (quiet == 0 && (argc <= 1)) {
		print_index(&cfg, &db);
	}

END:
	if((res = close_id()))
		top_level_error(ERRMAIN"close_id(): %d\n", res);

	if((res = close_config(&cfg)))
		top_level_error(ERRMAIN"close_config(): %d\n", res);

	close_db(&db);

	return EXIT_SUCCESS;
}
