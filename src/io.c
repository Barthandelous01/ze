#include <string.h>
#include <stdlib.h>

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
