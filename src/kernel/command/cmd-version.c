/*
 * kernel/command/cmd-version.c
 *
 * Copyright(c) 2007-2016 Jianjun Jiang <8192542@qq.com>
 * Official site: http://xboot.org
 * Mobile phone: +86-18665388956
 * QQ: 8192542
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <command/command.h>

static void usage(void)
{
	printf("usage:\r\n");
	printf("    version\r\n");
}

static int do_version(int argc, char ** argv)
{
	struct machine_t * mach = get_machine();
	int i;

	for(i = 0; i < 5; i++)
		printf("%s\r\n", xboot_character_logo_string(i));
	printf("%s - [%s][%s]\r\n", xboot_banner_string(), mach ? mach->name : "unknown", mach ? mach->desc : "none");
	return 0;
}

static struct command_t cmd_version = {
	.name	= "version",
	.desc	= "show xboot version information",
	.usage	= usage,
	.exec	= do_version,
};

static __init void version_cmd_init(void)
{
	register_command(&cmd_version);
}

static __exit void version_cmd_exit(void)
{
	unregister_command(&cmd_version);
}

command_initcall(version_cmd_init);
command_exitcall(version_cmd_exit);
