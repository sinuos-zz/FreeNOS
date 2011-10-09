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

#include "string.h"

int memcmp(const void *s1, const void *s2, size_t count)
{
    const char *p1 = s1, *p2 = s2;

    while (count > 0)
    {
	if (*p1 != *p2)
	{
	    return *p1 - *p2;
	}
	count--;
	p1++;
	p2++;
    }
    return 0;
}

