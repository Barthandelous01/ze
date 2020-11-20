#ifndef DB_H_
#define DB_H_

#include <lmdb.h>

typedef struct db_context {
	/* cursor to the DB */
	MDB_cursor *cursor;
	/* The transaction (only commits at close_db()) */
	MDB_txn *txn;
	/* generic db handle */
	MDB_dbi dbi;
	/* env handle. Must be allocated, opened, and closed seperately */
	MDB_env *env;
} db_context;

int init_db(db_context *DB);
int close_db(db_context *DB);

int add_record(db_context *DB, char *key, char *value);
int delete_record(db_context *DB, char *key);
int get_record(db_context *DB, char *key, char *value);

#endif	/* DB_H_ */
