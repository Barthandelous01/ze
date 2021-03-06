#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include <lmdb.h>

#include "error.h"
#include "io.h"

#include "db.h"

/**
 * start the database
 *
 * @param [in/out] DB: the context that is initialized
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

	if(mdb_cursor_open(DB->txn, DB->dbi, &DB->cursor))
		return -EDBCUR;

	return SUCCESS;
}

/**
 * shut down the database
 *
 * @param[in] DB: The DB handle
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
 * add a key-value pair to the db
 *
 * @param[in] DB: the DB context
 * @param[in] key: the key to insert under
 * @param[in] value: the value to insert
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
 * retreive a value from a key in the db
 *
 * @param[in] DB: the DB context
 * @param[in] key: the key to get the value of
 * @param[out] value: result buffer for the key
 *
 * Returns SUCCESS on success, or a negative code on
 * failure.
 */
int get_record(db_context *DB, char *key, char *value)
{
	MDB_val ke, val, temp;

	ke.mv_data = key;
	ke.mv_size = strlen(key);

	/* Oh, how I wish I'd picked a different database... Here's the rub. */
	/* LMDB only supports one-directional reads. And that's all. You go  */
	/* one way through, no choice about it. To go back requires setting  */
	/* your cursor back to the previous one, and while-loops are rather  */
	/* inefficient. Therefore, you use the nightmarish MDB_FIRST cursor  */
	/* operation to go back to the beginning of the database. That's why */
	/* there's this duplicity of operations. Since we can't sort CLI     */
	/* args, we can't just sort them and display them sequentially,      */
	/* either. So, we go back to the beginning of the DB *every stinging */
	/* time* just to make sure we don't get any weird non-extant zettel  */
	/* errors even when the zettel do exist.                             */
	/*                                                                   */
	/* Welcome to the wonderful world of C. We hope you like it here.    */
	if (mdb_cursor_get(DB->cursor, &temp, NULL, MDB_FIRST))
		return -EDBCUR;

	if(mdb_cursor_get(DB->cursor, &ke, &val, MDB_SET_KEY))
		return -EDBCUR;

	strncpy(value, val.mv_data, val.mv_size);
	value[val.mv_size] = '\0';

	return SUCCESS;
}

/**
 * delete a record from the db
 *
 * @param[in] DB: the DB context
 * @param[in] key: the key to delete
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
