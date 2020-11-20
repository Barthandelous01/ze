#ifndef DB_H_
#define DB_H_

int init_db();
int close_db();

int add_record(char *key, char *value);
int delete_record(char *key);
int get_record(char *key, char *value);

#endif	/* DB_H_ */
