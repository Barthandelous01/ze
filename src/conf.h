#ifndef CONF_H_
#define CONF_H_

#include "error.h"

typedef struct config_item config_item;

/**
 * A config item for a singly linked list hash map.
 */
struct config_item {
	/** The config item's key value */
	char key[CONF_KEY_SIZE];
	/** The config item's value */
	char value[CONF_KEY_SIZE];
	/** Pointer to the next config item */
	config_item *next;
};

/**
 * A really painful and simple config wrapper.
 * Keeps interface-facing functions from having to deal with raw
 * config_item-s.
 */
typedef struct config {
	config_item *head;
} config;

int parse_config(char *pathname, config *cfg);
int close_config(config *cfg);
int get_config(config *cfg, char *key, char *val);
void append_config(config *cfg, config_item *item);

#endif	/* CONF_H_ */
