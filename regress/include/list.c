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

#include <assert.h>
#include <list.h>

struct parent
{
    LIST_HEAD(child_head, child) childs;
};

struct child
{
    int id;
    LIST_ENTRY(child) next;
};

int main(int argc, char **argv)
{
    struct parent p1;
    struct child  c1, c2, c3;

    /* Initialize a list of childs. */
    c1.id = 1;
    c2.id = 2;
    c3.id = 3;

    /* Initialize the list head at parent. */
    LIST_INIT(&p1.childs);
    assert(LIST_EMPTY(&p1.childs));
    assert(LIST_FIRST(&p1.childs) == NULL);
    assert(LIST_LAST(&p1.childs)  == NULL);

    /* Add first child at the head. */
    LIST_INSERT_HEAD(&p1.childs, &c1, next);
    assert(!LIST_EMPTY(&p1.childs));
    assert(LIST_FIRST(&p1.childs) == &c1);
    assert(LIST_LAST(&p1.childs)  == &c1);
    assert(LIST_NEXT(&c1, next)   == NULL);
    assert(LIST_PREV(&c1, next)   == NULL);
    return 0;
}
