#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include <lmdb.h>

#include "error.h"
#include "io.h"

#include "db.h"

/**
 * init_db() - start the database
 * @DB: the context that is initialized
 *
 * Many DBs are fairly complicated to keep track of (commits, open
 * instances, etc) but LMDB is a cut above anything else I've
 * ever used. Each DB requires 4 structs at once... so init_db()
 * and close_db() use struct db_context to keep all of the items
 * we need for a working DB right next to each other.
 */
int init_db(db_context *DB)
{
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
int close_db(db_context *DB)
{
	mdb_cursor_close(DB->cursor);
	mdb_txn_commit(DB->txn);
	mdb_env_close(DB->env);

	return SUCCESS;
}

/**
 * add_record() - add a key-value pair to the db
 * @DB the DB context
 * @key: the key to insert under
 * @value: the value to insert
 *
 * The LMDB API is fairly complicated and unweildy, so
 * each of these "wrapper calls" takes the db_context
 * struct as its first argument.
 */
int add_record(db_context *DB, char *key, char *value)
{
	MDB_val ke, val;

	ke.mv_data = key;
	ke.mv_size = strlen(key);

	val.mv_data = value;
	val.mv_size = strlen(value);

	if(mdb_cursor_put(DB->cursor, &ke, &val, 0))
		return -EDBCUR;

	return SUCCESS;
}

/**
 * get_record() - retreive a value from a key in the db
 * @DB: the DB context
 * @key: the key to get the value of
 * @value: result buffer for the key
 *
 * Returns SUCCESS on success, or a negative code on
 * failure.
 */
int get_record(db_context *DB, char *key, char *value)
{
	char ret[200];
	MDB_val ke, val;

	memset(value, 0, sizeof(value));

	ke.mv_data = key;
	ke.mv_size = strlen(key);

	
	if(mdb_get(DB->txn, DB->dbi, &ke, &val))
		return -EDBCUR;

	strncpy(value, val.mv_data, val.mv_size);
	return SUCCESS;
}

/**
 * delete_record() - delete a record from the db
 * @DB: the DB context
 * @key: the key to delete
 *
 * The DB used by `ze` doesn't have multiple schema or
 * duplicate keys, so we only need the key to remove a
 * record.
 *
 * Returns SUCCESS on success, or a negative code on
 * failure.
 */
int delete_record(db_context *DB, char *key)
{
	MDB_val ke;
	ke.mv_data = key;
	ke.mv_size = strlen(key);

	if(mdb_del(DB->txn, DB->dbi, &ke, NULL))
		return -EDBCUR;

	return SUCCESS;
}
