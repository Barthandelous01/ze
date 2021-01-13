#ifndef CONF_H_
#define CONF_H_

#include "error.h"

typedef struct config_item config_item;

struct config_item {
	char key[CONF_KEY_SIZE];
	char value[CONF_KEY_SIZE];
	config_item *next;
};

typedef struct config {
	config_item *head;
} config;

int parse_config(char *pathname, config *cfg);
int close_config(config *cfg);
int get_config(config *cfg, char *key, char *val);
void append_config(config *cfg, config_item *item);

#endif	/* CONF_H_ */
