#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <lmdb.h>

#include "error.h"
#include "io.h"

typedef struct db_context {
	MDB_cursor *cursor;
	MDB_txn *txn;
	MDB_dbi dbi;
	MDB_env *env;
} db;
static db *DB;

/**
 * init_db() - start the database
 * @path: the path for the DB. Should be a directory, and
 *        should start with /
 *
 * As with many DB, this one is initialized into a POD
 * struct that stores environment and cursor for ease of
 * use.
 */
int init_db()
{

	DB = (db *)malloc(sizeof(db));
	if (!DB)
		return -EMEM;

	if (mdb_env_create(&DB->env))
		return -EDBENV;

	char temp_path[2*PATH_BUFSIZE];
	home_prefix(HOME_DIR, temp_path);
	if (mdb_env_open(DB->env, (const char *)temp_path, 0, 0755))
		return -EDBENV;

	if (mdb_txn_begin(DB->env, NULL, 0, &DB->txn))
		return -EDBTRX;

	if (mdb_open(DB->txn, NULL, MDB_CREATE, &DB->dbi))
		return -EDBTRX;

	if (mdb_cursor_open(DB->txn, DB->dbi, &DB->cursor))
		return -EDBCUR;

	return SUCCESS;
}

/**
 * close_db() - shut down the database
 *
 * Cleans up the DB and frees memory.
 */
int close_db()
{
	/* stuff */
	mdb_cursor_close(DB->cursor);
	mdb_txn_commit(DB->txn);
	mdb_env_close(DB->env);

	free(DB);

	return SUCCESS;
}
