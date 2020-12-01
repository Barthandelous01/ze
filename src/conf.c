#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "io.h"

#include "conf.h"

/**
 * read_config_line() - reads one line of a config file
 * @fd: the file to read frome
 * @key: the buffer to write the key into
 * @val: the buffer to write the val into
 *
 * read_config_line() reads one line into a key-value pair for a
 * config_item. Returns SUCCESS on success, or negative on failure.
 */
static int read_config_line(FILE *fd, char *key, char *val)
{
	int i = 0, j = 0;
	/* get first key */
	for(; i < CONF_KEY_SIZE-1
		    && (j = fgetc(fd)) != '='
		    && j != EOF
		    && j != '#'; i++)
		key[i] = j;
	/* null terminate */
	key[i] = '\0';
	if (j == EOF)
		return EOF;

	int k = 0;
	/* get value */
	for(; k < CONF_KEY_SIZE-1
		    && (j = fgetc(fd))
		    && j != EOF
		    && j != '#'
		    && j != '\n'; k++)
		val[k] = j;
	/* null-terminate */
	val[k] = '\0';
	if(j == EOF)
		return EOF;
	/* success */
	return SUCCESS;
}

/**
 * append_config() - append a pair to the config struct
 * @cfg: the config * to append to
 * @item: the address of the (presumably heap-allocated) item to append
 *
 * Append *item to the *cfg linked list.
 */
static void append_config(config *cfg, config_item *item)
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
 * close_config() - release all the allocated memory
 * @cfg: the config to release
 *
 * close_config() free()s all of the memory allocated in parsing
 * the config file into a singly-linked list.
 */
int close_config(config *cfg)
{
	config_item *temp;
	config_item *temp2 = NULL;
	if(cfg->head == NULL)
		return SUCCESS;

	temp = cfg->head;
	temp2 = temp->next;
	do {
		free(temp);
		temp = temp2;
		if(temp2 != NULL)
			temp2 = temp2->next;

	} while(temp != NULL);

	return SUCCESS;
}


/**
 * parse_config() - parse a config file and return the values inside it
 * @pathname: the pathname. Canonicalized; have leading /
 * @cfg: the config struct to fill in
 *
 * parse_config() is the driver function for the config module. It reads
 * a simple unix-y key-value config file, and returns a singly-linked
 * list of the keys.
 */
int parse_config(char *pathname, config *cfg)
{
	char x[CONF_KEY_SIZE], y[CONF_KEY_SIZE];
	char pth[2*PATH_BUFSIZE];
	config_item *item;
	home_prefix(pathname, pth);

	struct stat st;
	if(stat(pth, &st) != 0)
		return SUCCESS;

	FILE *fd = fopen(pth, "r");
	if(!fd)
		return -EFILE;

	while(read_config_line(fd, x, y) != EOF) {
		item = malloc(sizeof(config_item));
		strncpy(item->key, x, CONF_KEY_SIZE);
		strncpy(item->value, y, CONF_KEY_SIZE);
		append_config(cfg, item);
	}

	if(fclose(fd))
		return -EFILE;

	return SUCCESS;
}

/**
 * get_config() - get the setting from a config key
 * @cfg: the config header
 * @key: the key to retreive
 * @val: the result buffer
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
