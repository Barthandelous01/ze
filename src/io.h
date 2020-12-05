#ifndef IO_H_
#define IO_H_

#include "conf.h"

void home_prefix(char *postfix, char *result);

int print_zettel(config *cfg, char *pathname);
int edit_zettel(config *cfg, char *pathname);

void ensure_directories();

#endif	/* IO_H_ */
