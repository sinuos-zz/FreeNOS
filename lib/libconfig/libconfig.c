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

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include "libconfig.h"

/** Internal error buffer. */
static char error_buffer[CONFIG_LEN];

/**
 * Determine if the given string is a comment.
 * @param line String to check.
 * @return Non-zero if comment, and zero otherwise.
 */
static int is_comment(char *line)
{
    return line[0] == '#';
}

/**
 * Determine if the given string opens a new configuration subtree.
 * @param line String to check.
 * @return Non-zero if subtree open, and zero otherwise.
 */
static int is_subtree_open(char *line)
{
    return line[0] == '{';
}

/**
 * Determine if the given string closes a configuration subtree.
 * @param line String to check.
 * @return Non-zero if subtree close, and zero otherwise.
 */
static int is_subtree_end(char *line)
{
    return line[0] == '}';
}

/**
 * Determine if the given character is special (i.e. a space, tab, or newline).
 * @param c Character to check.
 * @return Non-zero if special, and zero otherwise.
 */
static int is_special(char c)
{
    const char special[] = { ' ', '\r', '\n', '\t' };
    int i;

    for (i = 0; i < sizeof(special) / sizeof(char); i++)
    {
	if (special[i] == c)
	{
	    return 1;
	}
    }
    return 0;
}

/**
 * Create a new configuration item.
 * @param name Name of the configuration item.
 * @param value Value of the configuration item.
 * @param parent Optional parent configuration item.
 * @return Pointer to the configuration item on success, NULL on failure.
 */
static config_item_t * config_insert_item(char *name, char *value,
					  config_item_t *parent)
{
    config_item_t *item;

    /* Allocate a new configuration item. */
    if (!(item = (config_item_t *) malloc(sizeof(config_item_t))))
    {
        snprintf(error_buffer, sizeof(error_buffer),
		"failed to malloc() config_item_t: %s", strerror(errno));
	return NULL;
    }
    /* Fill the item. */
    memset(item, 0, sizeof(*item));
    snprintf(item->name,  CONFIG_LEN, "%s", name);
    snprintf(item->value, CONFIG_LEN, "%s", value);

    /* Add to parent, if any given. */
    if (parent)
    {
	/* Enlarge the childs array. */
	parent->childs = realloc(parent->childs,
	    (parent->num_childs + 1) * sizeof(config_item_t *));

	if (!parent->childs)
	{
	    snprintf(error_buffer, sizeof(error_buffer),
		    "failed to malloc() config_item_t: %s", strerror(errno));
	    free(item);
	    return NULL;
	}
	/* Add as a child. */
	parent->childs[parent->num_childs] = item;
	parent->num_childs++;
    }
    /* All done. */
    return item;
}

/**
 * Removes any special characters before and after a string.
 * @param buf String buffer to strip.
 * @return Zero if line has valid non-special content, non-zero otherwise.
 */
static int strip(char *buf)
{
    char *start, *cur, *end;

    /* Strip any prepended special characters. */
    for (start = buf; *start && is_special(*start); start++)
    {
        ;
    }
    /* Find the end of the line. */
    if (*start)
    {
	for (end = cur = start; *cur; cur++)
	{
	    if (!is_special(*cur))
	    {
	        end = cur;
	    }
	}
	/* Copy to the final buffer. */
	memmove(buf, start, (end - start) + 1);
	buf[(end - start) + 1] = 0;
	return 0;
    }
    return 1;
}

/**
 * Recursively release configuration item memory.
 * @param item Configuration item to release.
 */
static void config_release_item(config_item_t *item)
{
    int i;

    /* Release it's childs, if any. */
    for (i = 0; i < item->num_childs; i++)
    {
	config_release_item(item->childs[i]);
    }
    /* Release item. */
    if (item->num_childs > 0)
    {
	free(item->childs);
    }
    free(item);
}

/**
 * Read a stripped line from the given file.
 * @param fp Open file descriptor to the file to read.
 * @param buf Output buffer.
 * @param size Maximum number of bytes to read.
 * @return Zero on success and non-zero on failure or EOF.
 */
static int read_line(FILE *fp, char *buf, size_t size, int *count)
{
    /*
     * Attempt to retrieve a stripped line from the file.
     */
    while (1)
    {
	/* Read one line from the file. */
        if (fgets(buf, size, fp) == NULL)
	{
	    return 1;
	}
	/* Increment line count. */
	(*count)++;

	/* Only return stripped, non-comment lines. */
	if (strip(buf) == 0 && !is_comment(buf))
	{
	    return 0;
	}
    }
}

/**
 * Generate a configuration item tree.
 * @param conf Configuration context.
 * @param parent Parent configuration item.
 * @param fp Open file descriptor to the configuration file.
 * @return Zero on success and non-zero on failure.
 */
static int config_item_tree(config_t *conf, config_item_t *parent, FILE *fp)
{
    char line[1024], *equals, *name, *value;
    int count = 0;
    config_item_t *item, *last = NULL;

    /*
     * Read the entire file until EOF.
     */
    while (1)
    {
	/* Clear line. */
	memset(line, 0, sizeof(line));

	/* Read a single stripped line. */
	if (read_line(fp, line, sizeof(line), &count) != 0)
	{
	    break;
	}
	/* Fetch key = value pairs. */
	if ((equals = strchr(line, '=')) != NULL)
	{
	    name  = "";
	    value = "";
	    *equals = 0;

	    if (equals - line > 0)
	    {
		name = line;
		strip(name);
	    }
	    if (*(equals+1))
	    {
		value = equals + 1;
		strip(value);
	    }
	}
	/* Subtree opening. */
	else if (is_subtree_open(line))
	{
	    if (!last)
	    {
		snprintf(error_buffer, sizeof(error_buffer),
			"unexpected '{' on line %d\r\n", count);
		return 1;
	    }
	    if (config_item_tree(conf, last, fp) != 0)
	    {
		return 1;
	    }
	    else
		continue;
	}
	/* Subtree closure. */
	else if (is_subtree_end(line))
	{
	    return 0;
	}
	/* Config item name only. */
	else
	{
	    name  = line;
	    value = "";
	}
	/* Insert the item. */
	last = config_insert_item(name, value, parent);
    }
    /* All done. */
    return 0;
}

config_t * libconfig_parse(char *path, int type)
{
    FILE *fp;
    config_t *conf;
    config_item_t *root;

    /* Attempt to open the configuration file. */
    if ((fp = fopen(path, "r")) == NULL)
    {
	snprintf(error_buffer, sizeof(error_buffer),
		 "failed to fopen() `%s': %s", path, strerror(errno));
	return NULL;
    }
    /* Allocate config_t. */
    if ((conf = (config_t *) malloc(sizeof(config_t))) == NULL)
    {
	snprintf(error_buffer, sizeof(error_buffer),
		 "failed to malloc() config_t: %s", strerror(errno));
	fclose(fp);
	return NULL;
    }
    /* Clear. */
    memset(conf, 0, sizeof(*conf));

    /* Insert root item. */
    if (!(conf->root = config_insert_item("root", "", NULL)))
    {
	fclose(fp);
	return NULL;
    }
    /* Parse configuration items. */
    if (config_item_tree(conf, conf->root, fp) != 0)
    {
	free(conf);
	fclose(fp);
	return NULL;
    }
    /* Close file. */
    fclose(fp);
    return conf;
}

char * libconfig_error(void)
{
    return error_buffer;
}

char * libconfig_find(config_item_t *item, char *namefmt, ...)
{
    va_list args;
    char namebuf[1024], *token;
    int i;

    /* Construct full name to the configuration item. */
    va_start(args, namefmt);
    vsnprintf(namebuf, sizeof(namebuf), namefmt, args);
    va_end(args);

    /* Split in tokens separated by a colon. */
    token = strtok(namebuf, ":");

    /* Must start with the given item. */
    if (strcmp(item->name, token) != 0)
    {
	return NULL;
    }
    /*
     * Search the configuration space.
     */
    while (item)
    {
	if ((token = strtok(NULL, ":")) == NULL)
	{
	    break;
	}
	for (i = 0; i < item->num_childs; i++)
	{
	    if (strcmp(item->childs[i]->name, token) == 0)
	    {
		item = item->childs[i];
		break;
	    }
	    if (i == item->num_childs - 1)
	    {
		item = NULL;
		break;
	    }
	}
    }
    return item ? item->value : NULL;
}

void libconfig_free(config_t *config)
{
    config_release_item(config->root);
    free(config);
}

