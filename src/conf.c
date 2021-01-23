#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include "io.h"

#include "conf.h"

#include "lexer.h"
#include "parser.h"

/**
 * append a pair to the config struct
 *
 * @param[in] cfg: the config * to append to
 * @param[in] item: the address of the (presumably heap-allocated) item to append
 *
 * Append *item to the *cfg linked list.
 */
void append_config(config *cfg, config_item *item)
{
	config_item *temp;
	temp = cfg->head;
	if (temp == NULL) {
		cfg->head = item;
		cfg->head->next = NULL;
		return;
	}
	while(temp->next != NULL)
		temp = temp->next;

	temp->next = item;
	item->next = NULL;
}

/**
 * free a config item recursively
 *
 * @param[in] item: the item to recursively free
 *
 * In order to free a linked list, this function uses simple
 * recursion to chain calls to free.
 */
static void free_conf_item(config_item *item)
{
	if(item != NULL && item->next != NULL)
		free_conf_item(item->next);
	free(item);
}

/**
 * release all the allocated memory
 *
 * @param [in] cfg: the config to release
 *
 * close_config() free()s all of the memory allocated in parsing
 * the config file into a singly-linked list.
 */
int close_config(config *cfg)
{
	free_conf_item(cfg->head);

	return SUCCESS;
}

/**
 * parse a config file and return the values inside it
 *
 * @param[in] pathname: the pathname. Canonicalized; have leading /
 * @param [in] cfg: the config struct to fill in
 *
 * parse_config() is the driver function for the config module. It reads
 * a simple unix-y key-value config file, and returns a singly-linked
 * list of the keys.
 */
int parse_config(char *pathname, config *cfg)
{
	char pth[2*PATH_BUFSIZE];
	config_item *item;
	int fd;
	char *file;
	cfg->head = NULL;

	home_prefix(pathname, pth);

	void *scanner;
	YY_BUFFER_STATE state;

	if (yylex_init(&scanner))
		return -EVAL;

	struct stat st;
	if(stat(pth, &st) != 0)
		return SUCCESS;

	if ((fd = open(pth, O_RDONLY)) == 0)
		return -EFILE;

	if ((file = malloc(st.st_size+1)) == NULL)
		return -EMEM;

	read(fd, file, st.st_size);

	state = yy_scan_string(file, scanner);

	if (yyparse(cfg, scanner))
		return -EVAL;

	yy_delete_buffer(state, scanner);
	yylex_destroy(scanner);
	return SUCCESS;
}

/**
 * get the setting from a config key
 *
 * @param[in] cfg: the config header
 * @param[in] key: the key to retreive
 * @param[out] val: the result buffer
 *
 * get_config() returns the value associated with a key and writes it into
 * val. Returns SUCCESS on success, or -EVAL if the key is not found.
 */
int get_config(config *cfg, char *key, char *val)
{
	config_item *temp = cfg->head;
	while(temp != NULL) {
		if(strcmp(key, temp->key) == 0) {
			strncpy(val, temp->value, CONF_KEY_SIZE);
			return SUCCESS;
		}
		temp = temp->next;
	}

	return -EVAL;
}
