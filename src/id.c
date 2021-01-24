#include <config.h>

#include <stdio.h>

#include <stdlib.h>
#ifndef HAVE_MALLOC
#  error "ze requires a working `malloc' function"
#endif
#include <string.h>
#include <sys/stat.h>

#include "error.h"
#include "io.h"

typedef struct id_file {
	FILE *fd;
	unsigned int id;
	char filepath[2*PATH_BUFSIZE];
} id_file;

static id_file *id_fd;

/**
 * set up the magic id system
 *
 * @param[in] filepath: the file that the ID is stored in
 *
 * init_id() fills in the magic stack-allocated id_fd
 * global that the entire id.c file works with. It keeps
 * track of the current ID, as well as the file descriptor
 * associated with it. The filepath passed must be
 * absolute: if it is not, fopen() does not canonicalize
 * filenames, and init_id() will return -EFILE.
 *
 * Returns ESUCCESS on success, or a negative code on
 * error.
 */
int init_id (char *filepath)
{
	extern id_file *id_fd;
	id_fd = (id_file *)malloc(sizeof(id_file));
	if (!id_fd)
		return -EMEM;

	char tmp_filepath[2*PATH_BUFSIZE];
	home_prefix(filepath, tmp_filepath);
	strncpy(id_fd->filepath, tmp_filepath, (2*PATH_BUFSIZE)-1);
	id_fd->filepath[(2*PATH_BUFSIZE)-1] = '\0';

	FILE *fd_temp = fopen(id_fd->filepath, "r+");
	if (!fd_temp) {
		id_fd->fd = NULL;
		id_fd->id = 0x1;
		return SUCCESS;
	}

	id_fd->fd = fd_temp;

	unsigned int id_temp = 0;
	fscanf(id_fd->fd, ID_FORMAT, &id_temp);
	if (id_temp == 0)
		id_fd->id = 0x1;
	else
		id_fd->id = id_temp;

	return SUCCESS;
}


/**
 * close the magic id system
 *
 * close_id() shuts down all of the stuff inside of
 * id_fd, writes the ID update, and closes file descriptors.
 * Returns ESUCCESS on success, or a negative error code
 * on failure.
 */
int close_id (void)
{
	extern id_file *id_fd;
	if (id_fd->fd != NULL) {
		int res1 = fclose(id_fd->fd);
		if (res1)
			return -EFILE;
	}

	FILE *fd = fopen(id_fd->filepath, "w");
	if (!fd)
		return -EFILE;

	fprintf(fd, ID_FORMAT, id_fd->id);
	if (fclose(fd))
		return -EFILE;

	free(id_fd);

	return SUCCESS;
}

/**
 * add to the ID counter
 *
 * incriment_id() is basically id_fd.id++, but without having
 * to directly address the global, and is more obvious to
 * readers.
 */
static inline void incriment_id()
{
	extern id_file *id_fd;
	id_fd->id++;
}

/**
 * get the current ID integer
 *
 * get_id_int() returns the current ID ingeter.
 * The reason for it and increment_id() is to keep from
 * directly touching a global in a non-sane-way.
 */
static inline unsigned int get_id_int()
{
	extern id_file *id_fd;
	return id_fd->id;
}

/**
 * get a new ID string
 *
 * @param[out] result_buf: the stack buffer where the result is
 *              written to
 *
 * get_id() is the main userspace function from id. It
 * returns ESUCCESS on success, or a negative error code on
 * failure. The resultant char * is written to result_buf.
 */
int get_id (char *result_buf)
{
	extern id_file *id_fd;
	if (!result_buf)
		return -EMEM;

	sprintf(result_buf, ID_FORMAT, get_id_int());
	incriment_id();

	return SUCCESS;
}
