/*  Copyright (C) 2006 Hans de Goede <j.w.r.degoede@hhs.nl>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public Licence as published by
 *  the Free Software Foundation; either version 2 of the Licence, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public Licence for more details.
 *
 *  You should have received a copy of the GNU General Public Licence
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *  The GPL version 2 is included in this distribution in a file called
 *  COPYING. Use any text editor or text viewing command to read it. */
#include <stdarg.h>
#include <unistd.h>
#include <dirent.h>
#include <pwd.h>
#include <sys/stat.h>
#include <sys/types.h>

#ifdef BSD43	/* old style directory handling */
#include <sys/types.h>
#include <sys/dir.h>
#define dirent direct
#endif

#if defined(__DECC) && defined(VMS)
#include <unixlib.h>
static char *vms_to_unix_buffer = NULL;
static int convert_vms_to_unix(char *vms_dir_name)
{
	vms_to_unix_buffer = vms_dir_name;
}
#endif

/*
 * Locate user's home directory.
 */
char *get_home_dir(void)
{
	struct passwd *pw;

	if (!(pw = getpwuid(getuid()))) {
		fprintf(stderr, "Who are you? Not found in passwd database!!\n");
		return NULL;
	}

#if defined(__DECC) && defined(VMS)
	/* Convert The OpenVMS Formatted "$HOME" Directory Path Into Unix
	 * Format. */
	decc$from_vms(pw->pw_dir, convert_vms_to_unix, 1);
	return vms_to_unix_buffer;
#else
	return pw->pw_dir;
#endif
}


/*
 * Check and, if necessary, create dir.
 */
int check_and_create_dir(const char *name)
{
	struct stat stat_buffer;

	if (stat(name, &stat_buffer)) {
		/* error check if it doesn't exist or something else is wrong */
		if (errno == ENOENT) {
			/* doesn't exist letts create it ;) */
#ifdef BSD43
			if (mkdir(name, 0775))
#else
			if (mkdir(name, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH))
#endif
			{
				fprintf(stderr, "Error creating dir %s", name);
				perror(" ");
				return -1;
			}
		} else {
			/* something else went wrong yell about it */
			fprintf(stderr, "Error opening %s", name);
			perror(" ");
			return -1;
		}
	} else {
		/* file exists check it's a dir otherwise yell about it */
#ifdef BSD43
		if (!(S_IFDIR & stat_buffer.st_mode))
#else
		if (!S_ISDIR(stat_buffer.st_mode))
#endif
		{
			fprintf(stderr, "Error %s exists but isn't a dir\n", name);
			return -1;
		}
	}
	return 0;
}
