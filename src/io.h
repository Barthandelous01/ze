#ifndef IO_H_
#define IO_H_

void home_prefix(char *postfix, char *result);
int print_zettel(void *cfg, char *pathname);
int edit_zettel(void *cfg, char *pathname);

#endif	/* IO_H_ */
