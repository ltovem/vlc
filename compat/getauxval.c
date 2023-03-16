// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * getauxval.c: Linux getauxval() replacement
 *****************************************************************************
 * Copyright © 2022 Rémi Denis-Courmont
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stddef.h>

extern char **environ;

unsigned long getauxval(unsigned long type)
{
	const unsigned long *auxv;
	size_t i = 0;

	while (environ[i++] != NULL);

	auxv = (const void *)&environ[i];

	for (i = 0; auxv[i]; i += 2)
		if (auxv[i] == type)
			return auxv[i + 1];

	return 0;
}
