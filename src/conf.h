#ifndef CONF_H_
#define CONF_H_

#include "error.h"

typedef struct config_item config_item;

typedef struct config_item {
	char key[CONF_KEY_SIZE];
	char value[CONF_KEY_SIZE];
	config_item *next;
} config_item;

typedef struct config {
	config_item *head;
} config;

int parse_config(char *pathname, config *cfg);
int close_config(config *cfg);

#endif	/* CONF_H_ */
