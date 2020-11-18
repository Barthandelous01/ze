#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "error.h"

typedef struct id_file {
	FILE *fd;
	unsigned int id;
	char filepath[PATH_BUFSIZE];
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
 *
 * Returns ESUCCESS on success, or a negative code on
 * error.
 */
int init_id (char *filepath)
{
	FILE *fd_temp = fopen(filepath, "r");
	if (!fd_temp)
		return -EFILE;

	id_fd.fd = fd_temp;
	strncpy(id_fd.filepath, filepath, PATH_BUFSIZE-1);
	id_fd.filepath[49] = '\0';

	unsigned int id_temp = 0;
	fscanf(id_fd.fd, "%6x", &id_temp);
	if (id_temp == 0)
		id_fd.id = 1;
	else
		id_fd.id = id_temp;

	return SUCCESS;
}

/**
 * close_id() - close the magic id system
 *
 * close_id() shuts down all of the stuff inside of
 * id_fd, writes the ID update, and closes file descriptors.
 * Returns ESUCCESS on success, or a negative error code
 * on failure.
 */
int close_id (void)
{
	int res1 = fclose(id_fd.fd);
	if (!res1)
		return -EFILE;

	FILE *fd = fopen(id_fd.filepath, "w");
	if (!fd)
		return -EFILE;

	fprintf(fd, "%6x", id_fd.id);
	if (!fclose(fd))
		return -EFILE;

	return SUCCESS;
}

/**
 * incriment_id() - add to the ID counter
 *
 * incriment_id() is basically id_fd.id++, but without having
 * to directly address the global, and is more obvious to
 * readers.
 */
static inline void incriment_id()
{
	id_fd.id++;
}

/**
 * get_id_int() - get the current ID integer
 *
 * get_id_int() returns the current ID ingeter.
 * The reason for it and increment_id() is to keep from
 * directly touching a global in a non-sane-way.
 */
static inline unsigned int get_id_int()
{
	return id_fd.id;
}

/**
 * get_id() - get a new ID string
 * @result_buf: the stack buffer where the result is
 *              written to
 *
 * get_id() is the main userspace function from id. It
 * returns ESUCCESS on success, or a negative error code on
 * failure. The resultant char * is written to result_buf.
 */
int get_id (char *result_buf)
{
	if (!result_buf)
		return -EMEM;

	sprintf(result_buf, "%6x", get_id_int());
	incriment_id();

	return SUCCESS;
}
