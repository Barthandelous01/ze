#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdarg.h>
#include <sys/wait.h>

#include "error.h"
#include "conf.h"

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
 * raw_print() - print a raw file
 * @pathname: the path to the zettel to print
 *
 * Efficiently print a zettel file. Uses buffering (similar to
 * cat(1)) in order to print the zettel as fast as possible.
 * The methods are influeneced by GNU yes.
 */
static int raw_print(char *pathname)
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

/**
 * print_zettel() - print a zettel
 * @cfg: unused. To pass config to.
 * @pathname: the path of the zettel to print. Should
 *            have a leading /
 */
int print_zettel(config *cfg, char *pathname)
{
	char path[2*PATH_BUFSIZE];
	char conf_pager[CONF_KEY_SIZE] = "";
	int pid, res;

	home_prefix(pathname, path);
	res = get_config(cfg, "PAGER", conf_pager);
	if((pid = fork()) == 0) {
		if (res != SUCCESS) {
			if(execl(DEF_PAGE, path, (char *)NULL)) {
				if (raw_print(path))
					return -EFILE;
			}
		} else {
			if(execl(conf_pager, conf_pager, path, (char *) NULL)) {
				if(raw_print(path))
					return -EFILE;
			}
		}
		exit(EXIT_SUCCESS);
	} else if (pid < 0) {
		return -EFORK;
	} else {
		wait(NULL);
	}

	return SUCCESS;
}

/**
 * edit_zettel() - edit a zettel
 * @cfg: unused. To pass config to.
 * @pathname: the path of the zettel to edit.
 *            Is canonicalized; lead with /
 */
int edit_zettel(void *cfg, char *pathname)
{
	char path[2*PATH_BUFSIZE];
	char conf_editor[CONF_KEY_SIZE];
	int pid;

	home_prefix(pathname, path);
	get_config(cfg, "EDITOR", conf_editor);
	if ((pid = fork()) == 0) {
		if(strcmp(conf_editor, "") == 0) {
			if(execl(getenv("EDITOR"), getenv("EDITOR"), path, (char *)NULL))
				execl(DEF_EDIT, path, (char *)NULL);
		} else {
			if(execl(conf_editor, conf_editor, path, (char *)NULL))
				execl(DEF_EDIT, path, (char *)NULL);
		}
		exit(EXIT_SUCCESS);
	} else if (pid < 0) {
		return -EFORK;
	} else {
		wait(NULL);
	}

	return SUCCESS;
}

/**
 * check_dir() - ensure directories exist
 * @dir: the directory to check
 *
 * do a quick and dirty check to see if `dir` exists
 */
static void check_dir(char *dir)
{
	char path[2*PATH_BUFSIZE];
	home_prefix(dir, path);
	struct stat st = {0};
	if (stat(path, &st) == -1)
		mkdir(path, 0777);
}

/**
 * ensure_directories() - run a check on each directory needed by ze
 *
 * A relatively inelegant way to make sure directories exist
 * (as C does not, unfortunately, have a ensure-directories function as
 * lisp does)
 */
void ensure_directories()
{
	 check_dir(HOME_DIR);
	 check_dir(ZETTEL_DIR);
}
