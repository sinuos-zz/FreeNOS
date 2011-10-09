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
#include <libconfig.h>
#include <libexec.h>
#include <libboot.h>
#include <FreeNOS/version.h>

/**
 * Generate a boot image using a configuration file.
 * @param conf Boot image configuration file.
 * @param built_root Root directory containing binary executable programs.
 * @param image_path Path to the output image.
 */
static void build_image(config_t *conf, char *build_root,
			char *image_path)
{
    int i,j;
    char path[1024];
    unsigned short flags = 0;
    program_t *program;

    /* Initialize libraries. */
    libexec_init();
    libboot_init();

    /* Loop all configured boot programs. */
    for (i = 0; i < conf->root->num_childs; i++)
    {
        /* Construct path to the program inside buildroot. */
	snprintf(path, sizeof(path), "%s/%s",
		 build_root, conf->root->childs[i]->name);

        /* Parse the program's loadable memory regions. */
	if ((program = libexec_read(path)) == NULL)
	{
	    printf("libexec_read() failed: %s\r\n", libexec_error());
	    continue;
	}
	/* Look for the idle flag. */
	if (libconfig_find(conf->root, "root:%s:isidle",
			   conf->root->childs[i]->name))
	{
	    flags |= BOOTPROG_ISIDLE;
	}
	/* Add it to the boot image. */
	libboot_addprog(libconfig_find(conf->root, "root:%s:cmd",
				       conf->root->childs[i]->name),
			program->entry, flags);

        /* Add all it's memory regions. */
	for (j = 0; j < program->num_regions; j++)
	{
	    libboot_addseg(program->regions[j].base,
	                   program->regions[j].size,
	                   program->regions[j].data);
	}
	libexec_free(program);
	flags = 0;
    }
    /* Write the final image. */
    libboot_write(image_path);

    /* Deinitialize libraries. */
    libboot_deinit();
    libexec_deinit();
}

int main(int argc, char **argv)
{
    config_t *conf;

    /* Verify command line arguments. */
    if (argc < 4)
    {
	printf("usage: %s CONFIG BUILDROOT IMAGE\r\n", argv[0]);
	return EXIT_FAILURE;
    }
    /* Parse configuration. */
    if ((conf = libconfig_parse(argv[1], CONFIG_TYPE_TREE)) == NULL)
    {
	printf("%s: %s\r\n", argv[0], libconfig_error());
	return EXIT_FAILURE;
    }
    /* Build an image from the given configuration. */
    build_image(conf, argv[2], argv[3]);

    /* Release configuration. */
    libconfig_free(conf);

    /* Success. */
    return EXIT_SUCCESS;
}

