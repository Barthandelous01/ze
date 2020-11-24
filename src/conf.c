#include <stdio.h>
#include <stdlib.h>

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
	size_t i = 0, j = 0;
	/* get first key */
	for(i, j; i < CONF_KEY_SIZE
		    && (j = fgetc(fd)) != '='
		    && j != EOF
		    && j != '#'; i++)
		key[i] = j;
	/* null terminate */
	key[i++] = '\0';
	if (j == EOF)
		return EOF;

	size_t k = 0;
	/* get value */
	for(k, j; k < CONF_KEY_SIZE
		    && (j = fgetc(fd))
		    && j != EOF
		    && j != '#'
		    && j != '\n'; k++)
		val[k] = j;
	/* null-terminate */
	val[k++] = '\0';
	if(j == EOF)
		return EOF;
	/* success */
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
	home_prefix(pathname, pth);

	return SUCCESS;
}
