#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "error.h"

/**
 * home_prefix() - $HOME-prefix a filepath
 * @postfix: the filepath to prefix. It should
 *           start with a /
 * @result: the buffer written into. It is assumed to
 *          have 2*PATH_BUFSIZE bytes maximum; otherwise,
 *          data will be lost. However, this should be
 *          enough for 99.9% of use cases.
 *
 * home_prefix() attempts to accomplish it's task
 * entirely without using the heap,
 * essentially canonizalizing filenames.
 */
void home_prefix(char *postfix, char *result)
{
	strncpy(result, getenv("HOME"), PATH_BUFSIZE);
	strncat(result, postfix, PATH_BUFSIZE-1);
	result[(2*PATH_BUFSIZE)-1] = '\0';
}

/**
 * print_zettel() - print a zettel
 * @pathname: the path to the zettel to print
 *
 * Efficiently print a zettel file. Uses buffering (similar to
 * cat(1)) in order to print the zettel as fast as possible.
 * The methods are influeneced by GNU yes.
 */
int print_zettel(char *pathname)
{
	int fd = open(pathname, O_RDONLY);
	if (!fd)
		return -EFILE;

	struct stat buf;
	fstat(fd, &buf);
	char buffer[BUF_CHUNKSIZE];

	if(buf.st_size < BUF_CHUNKSIZE) {
                /* The file fits in one chunk */
		read(fd, buffer, buf.st_size);
		write(fileno(stdout), buffer, buf.st_size);
	} else {
		/* we need to do more than one read */
		do {
			memset(buffer, 0, BUF_CHUNKSIZE);
			if(read(fd, buffer, BUF_CHUNKSIZE) == 0)
				break;
		} while(write(fileno(stdout), buffer, BUF_CHUNKSIZE)
			== BUF_CHUNKSIZE);
	}

	if(close(fd))
		return -EFILE;

	return SUCCESS;
}
