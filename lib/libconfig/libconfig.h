/*
 * Copyright (C) 2010 Niek Linnenbank
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __LIBCONFIG_H
#define __LIBCONFIG_H

#include <sys/types.h>

/** Key = Value configuration style. */
#define CONFIG_TYPE_KEYVALUE (1 << 0)

/** item { ... } configuration style. */
#define CONFIG_TYPE_TREE     (1 << 1)

/** Maximum length of name and value in items. */
#define CONFIG_LEN 128

/**
 * Represents a single configuration item.
 */
typedef struct config_item
{
    /** Name of the configuration item. */
    char name[CONFIG_LEN];

    /** Value of the config item. */
    char value[CONFIG_LEN];

    /** Parent item. */
    struct config_item *parent;

    /** Array containing any number of child items. */
    struct config_item **childs;

    /** Number of childs items. */
    size_t num_childs;
}
config_item_t;

/**
 * Represents a (parsed) configuration file.
 */
typedef struct config
{
    /** First (mandatory) root configuration item. */
    config_item_t *root;
}
config_t;

/**
 * Parse a configuration file.
 * @param path Path to the configuration file.
 * @param type Configuration style type to use.
 * @return config_t pointer on success and NULL on failure.
 */
extern config_t * libconfig_parse(char *path, int type);

/**
 * Get the string value of the last known error.
 * @return String pointer.
 */
extern char * libconfig_error(void);

/**
 * Retrieve the value of the given formatted configuration item name.
 *
 * The formatted name includes all configuration item names
 * from the given item to the target item separated by a ':'
 * characters. For example: 'root:server:core:ipaddr'.
 * It is allowed to use special printf() formats like %s, %d etc.
 *
 * @param item Configuration item to start look in.
 * @param namefmt Formatted configuration item name.
 * @param ... Optional argument list.
 * @return Pionter to the value of the given configuration item name
 *         if found, and NULL otherwise.
 */
extern char * libconfig_find(config_item_t *item, char *namefmt, ...);

/**
 * Release memory associated with a configuration.
 * @param config Parsed configuration.
 */
extern void libconfig_free(config_t *config);

#endif /* __LIBCONFIG_H */

