#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "error.h"

typedef struct id_file {
	FILE *fd;
	int id;
	char filepath[50];
} id_file;
static id_file id_fd;

/**
 * init_id() - set up the magic id system
 * @filepath: the file that the ID is stored in
 *
 * init_id() fills in the magic stack-allocated id_fd
 * global that the entire id.c file works with. It keeps
 * track of the current ID, as well as the file descriptor
 * associated with it.
 */
int init_id (char *filepath)
{
	FILE *fd_temp = fopen(filepath, "r+");
	if (!fd_temp)
		return -EFILE;

	id_fd.fd = fd_temp;
	strncpy(id_fd.filepath, filepath, 49);
	id_fd.filepath[49] = '\0';

	int id_temp;
	fscanf(id_fd.fd, "%x", id_temp);
	if (id_temp < 0)
		return -EVAL;
	else
		id_fd.id = id_temp;

	return ESUCCESS;
}

/**
 * close_id() - close the magic id system
 *
 * close_id() shuts down all of the stuff inside of
 * id_fd, writes the ID update, and closes file descriptors.
 */
int close_id ()
{
	int res1 = fclose(id_fd.fd);
	if (!res1)
		return -EFILE;

	FILE *fd = fopen(id_fd.filepath, "w");
	if (!fd)
		return -EFILE;

	fprintf(fd, "%x", id_fd.id);
	if (!fclose(fd))
		return -EFILE;

	return ESUCCESS;
}
