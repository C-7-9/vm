/*
 * Copyright (C) 2023 Woohyun Joh <jeremiahjoh@sungkyul.ac.kr>
 *
 * This file is part of DEW.
 *                                                                        
 * DEW is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *                                                                        
 * DEW is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *                                                                        
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "str.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

char *
str_new(const char *str)
{
	size_t len = strlen(str) + 1;
	char *new_str = (char *)malloc(len);
	strcpy(new_str, str);
	return new_str;
}

int
str_replace(char **old, char **new)
{
	*old = *new;
	*new = str_new("");

	return 0;
}

int 
str_get_index(char **str, char *ch, size_t idx)
{
	if (str == NULL)
		return -1;
	if (strlen(*str) < idx)
		return -1;
	
	*ch = (*str)[idx];
	return 0;
}

int 
str_set_index(char **str, char ch, size_t idx)
{
	if (str == NULL)
		return -1;
	if (strlen(*str) < idx)
		return -1;
	
	(*str)[idx] = ch;
	return 0;
}

int 
str_insert(char **str, char ch, size_t idx)
{
	if (str == NULL)
		return -1;

	size_t tot_len = strlen(*str) + 1;
	if (tot_len < idx)
		return -1;
	
	*str = realloc(*str, tot_len);
	for (int i = tot_len - 1; i > idx; i--)
		(*str)[i] = (*str)[i - 1];
	(*str)[idx] = ch;
	return 0;
}

int
str_insert_str(char **str, char *sub, size_t idx)
{
	if (str == NULL)
		return -1;
	
	size_t str_len = strlen(*str);
	size_t sub_len = strlen(sub);
	size_t tot_len = str_len + sub_len + 1;

	*str = realloc(*str, tot_len);
	for (int i = tot_len - 1; i > idx; i--)
		(*str)[i] = (*str)[i - sub_len];
	for (int i = idx; i < idx + sub_len; i++)
		(*str)[i] = sub[i - idx];

	return 0;
}

int 
str_remove(char **str, char *ch, size_t idx)
{
	if (str == NULL)
		return -1;

	size_t len = strlen(*str);
	if (len < idx)
		return -1;
	
	*ch = (*str)[idx];
	for (int i = idx; i < len; i++)
		(*str)[i] = (*str)[i + 1];
	*str = realloc(*str, len - 1);
	return 0;
}

int
str_remove_str(char **str, char *rmv)
{
	size_t str_len = strlen(*str);
	size_t rmv_len = strlen(rmv);
	for (int i = 0; i < str_len; i++)
		if (!memcmp((*str) + i, rmv, rmv_len)) {
			int idx = i;
			for (; i < str_len - rmv_len; i++)
				(*str)[i] = (*str)[i + rmv_len];
			(*str)[i] = '\0';

			*str = realloc(*str, str_len - rmv_len);
			return idx;
		}

	return -1;
}

int 
str_push(char **str, char ch)
{
	if (str == NULL)
		return -1;

	size_t len = strlen(*str);
	*str = realloc(*str, len + 2);
	(*str)[len] = ch;
	(*str)[len + 1] = '\0';
	return 0;
}

int
str_push_str(char **l_str, char *r_str)
{
	if (l_str == NULL || r_str == NULL)
		return -1;
	
	size_t r_len = strlen(r_str);
	size_t l_len = strlen(*l_str);
	*l_str = realloc(*l_str, l_len + r_len + 1);
	memcpy(*l_str + l_len, r_str, r_len);
	(*l_str)[l_len + r_len] = '\0';
	return 0;
}

int 
str_pop(char **str, char *ch)
{
	if (str == NULL)
		return -1;

	size_t len = strlen(*str);
	*ch = (*str)[len - 1];
	(*str)[len - 1] = '\0';
	*str = realloc(*str, len);
	return 0;
}

int
str_pop_str(char **str, char **pop, size_t idx)
{
	if (str == NULL || pop == NULL)
		return -1;
	
	size_t str_len = strlen(*str);
	size_t pop_len = str_len - idx;

	*pop = realloc(*pop, pop_len + 1);
	strcpy(*pop, *str + idx);
	pop[pop_len - 1] = '\0';

	return 0;
}
