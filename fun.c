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

#include "fun.h"
#include "str.h"
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#define BUF_LEN 50

int
value_bool_and(struct value stack[STACK_LEN], size_t *len)
{
	if (*len < 2)
		return -1;
	
	struct value *top = &stack[*len - 1];
	struct value *sec = &stack[*len - 2];
	if (top->type != BOOL || sec->type != BOOL)
		return -1;
	
	sec->data.bool &= top->data.bool;
	(*len)--;
	return 0;
}

int
value_bool_oor(struct value stack[STACK_LEN], size_t *len)
{
	if (*len < 2)
		return -1;

	struct value *top = &stack[*len - 1];
	struct value *sec = &stack[*len - 2];
	if (top->type != BOOL || sec->type != BOOL)
		return -1;
	
	sec->data.bool |= top->data.bool;
	(*len)--;
	return 0;
}

int
value_bool_xor(struct value stack[STACK_LEN], size_t *len)
{
	if (*len < 2)
		return -1;

	struct value *top = &stack[*len - 1];
	struct value *sec = &stack[*len - 2];
	if (top->type != BOOL || sec->type != BOOL)
		return -1;
	
	sec->data.bool ^= top->data.bool;
	(*len)--;
	return 0;
}

int
value_bool_not(struct value stack[STACK_LEN], size_t *len)
{
	if (*len < 1)
		return -1;
	
	struct value *top = &stack[*len - 1];
	if (top->type != BOOL)
		return -1;
	
	top->data.bool = !top->data.bool;
	return 0;
}

int
value_real_add(struct value stack[STACK_LEN], size_t *len)
{
	if (*len < 2)
		return -1;

	struct value *top = &stack[*len - 1];
	struct value *sec = &stack[*len - 2];
	if (top->type != REAL || sec->type != REAL)
		return -1;

	sec->data.real += top->data.real;
	(*len)--;
	return 0;
}

int
value_real_sub(struct value stack[STACK_LEN], size_t *len)
{
	if (*len < 2)
		return -1;
	
	struct value *top = &stack[*len - 1];
	struct value *sec = &stack[*len - 2];
	if (top->type != REAL || sec->type != REAL)
		return -1;

	sec->data.real -= top->data.real;
	(*len)--;
	return 0;
}

int
value_real_mul(struct value stack[STACK_LEN], size_t *len)
{
	if (*len < 2)
		return -1;

	struct value *top = &stack[*len - 1];
	struct value *sec = &stack[*len - 2];
	if (top->type != REAL || sec->type != REAL)
		return -1;

	sec->data.real *= top->data.real;
	(*len)--;
	return 0;
}

int
value_real_div(struct value stack[STACK_LEN], size_t *len)
{
	if (*len < 2)
		return -1;
	
	struct value *top = &stack[*len - 1];
	struct value *sec = &stack[*len - 2];
	if (top->type != REAL || sec->type != REAL)
		return -1;

	sec->data.real /= top->data.real;
	(*len)--;
	return 0;
}

int
value_real_rem(struct value stack[STACK_LEN], size_t *len)
{
	if (*len < 2)
		return -1;
	
	struct value *top = &stack[*len - 1];
	struct value *sec = &stack[*len - 2];
	if (top->type != REAL || sec->type != REAL)
		return -1;

	sec->data.real 
		= fmod(top->data.real, sec->data.real);
	(*len)--;
	return 0;
}

int
value_text_append(struct value stack[STACK_LEN], size_t *len)
{
	if (*len < 2)
		return -1;
	
	struct value *top = &stack[*len - 1];
	struct value *sec = &stack[*len - 2];
	if (top->type != TEXT || sec->type != TEXT)
		return -1;
	
	str_push_str(&sec->data.text, top->data.text);
	(*len)--;
	return 0;
}

int
value_text_insert(struct value stack[STACK_LEN], size_t *len)
{
	if (*len < 3)
		return -1;
	
	struct value *top = &stack[*len - 1];
	struct value *sec = &stack[*len - 2];
	struct value *bot = &stack[*len - 3];
	if (top->type != REAL
		|| sec->type != TEXT
		|| bot->type != TEXT)
		return -1;
	
	str_insert_str(&bot->data.text,
				   sec->data.text,
				   (size_t)top->data.real);
	*len -= 2;
	return 0;
}

int
value_text_remove(struct value stack[STACK_LEN], size_t *len)
{
	if (*len < 2)
		return -1;

	struct value *top = &stack[*len - 1];
	struct value *sec = &stack[*len - 2];
	if (top->type != TEXT || sec->type != TEXT)
		return -1;
	
	str_remove_str(&sec->data.text,
			   	   top->data.text);
	(*len)--;
	return 0;
}

int
value_text_search(struct value stack[STACK_LEN], size_t *len)
{
	if (*len < 2)
		return -1;
	
	struct value *top = &stack[*len - 1];
	struct value *sec = &stack[*len - 2];
	if (top->type != TEXT || sec->type != TEXT)
		return -1;
	
	char *ptr = strstr(sec->data.text, top->data.text);
	if (ptr == NULL)
		*sec = value_real_null();
	else
		*sec = value_real_with((double)(ptr - sec->data.text));
	(*len)--;
	return 0;
}

int
value_text_getnth(struct value stack[STACK_LEN], size_t *len)
{
	if (*len < 2)
		return -1;
	
	struct value *top = &stack[*len - 1];
	struct value *sec = &stack[*len - 2];
	if (sec->type != TEXT || top->type != REAL)
		return -1;
	
	size_t idx = (size_t)top->data.real;
	char buf[] = " ";
	char ch;
	if (str_get_index(&sec->data.text, &ch, idx))
		*sec = value_text_null();
	else {
		buf[0] = ch;
		*sec = value_text_with(buf);
	}
	
	(*len)--;
	return 0;
}

int
value_text_setnth(struct value stack[STACK_LEN], size_t *len)
{
	if (*len < 3)
		return -1;
	
	struct value *top = &stack[*len - 1];
	struct value *sec = &stack[*len - 2];
	struct value *bot = &stack[*len - 3];
	if (top->type != REAL
		|| sec->type != TEXT
		|| bot->type != TEXT)
		return -1;
	if (strlen(sec->data.text) != 1)
		return -1;
	
	if (str_set_index(&bot->data.text,
					  sec->data.text[0],
					  (size_t)top->data.real))
		*bot = value_text_null();

	*len -= 2;
	return 0;
}

int
value_text_rmvnth(struct value stack[STACK_LEN], size_t *len)
{
	if (*len < 2)
		return -1;
	struct value *top = &stack[*len - 1];
	struct value *sec = &stack[*len - 2];

	if (sec->type != TEXT || top->type != REAL)
		return -1;

	size_t idx = (size_t)top->data.real;
	char ch;
	if (str_remove(&sec->data.text, &ch, idx))
		*sec = value_text_null();
	
	(*len)--;
	return 0;
}

int
value_text_length(struct value stack[STACK_LEN], size_t *len)
{
	if (*len < 1)
		return -1;

	struct value *top = &stack[*len - 1];
	if (top->type != TEXT)
		return -1;
	
	*top =value_real_with((double)strlen(top->data.text));
	return 0;
}

static inline int
is_list(struct value *val)
{
	return val->type == BOOL_L 
		|| val->type == REAL_L
		|| val->type == TEXT_L;
}

static inline int
invalid_item(struct value *list, struct value *item)
{
	return (list->type == BOOL_L && item->type != BOOL && item->type != BOOL_L)
		|| (list->type == REAL_L && item->type != REAL && item->type != REAL_L)
		|| (list->type == TEXT_L && item->type != TEXT && item->type != TEXT_L);
}

int
value_list_append(struct value stack[STACK_LEN], size_t *len)
{
	if (*len < 2)
		return -1;
	
	struct value *top = &stack[*len - 1];
	struct value *sec = &stack[*len - 2];
	if (!is_list(sec) || invalid_item(sec, top))
		return -1;

	size_t item_len = is_list(top) ? top->data.list.len : 1;
	size_t new_size = (sec->data.list.len + item_len) * sizeof(struct value);
	sec->data.list.value = realloc(sec->data.list.value, new_size);

	if (is_list(top))
		for (size_t i = 0; i < top->data.list.len; i++)
			sec->data.list.value[sec->data.list.len + i]
				= top->data.list.value[i];
	else
		sec->data.list.value[sec->data.list.len] = *top;

	sec->data.list.len += item_len;
	(*len)--;

	return 0;
}

int
value_list_insert(struct value stack[STACK_LEN], size_t *len)
{
	if (*len < 3)
		return -1;

	struct value *top = &stack[*len - 1];
	struct value *sec = &stack[*len - 2];
	struct value *bot = &stack[*len - 3];
	if (!is_list(bot) || invalid_item(bot, sec))
		return -1;

	size_t idx = (size_t)top->data.real;
	size_t item_len = is_list(sec) ? sec->data.list.len : 1;
	size_t new_size = (bot->data.list.len + item_len) * sizeof(struct value);
	bot->data.list.value = realloc(bot->data.list.value, new_size);

	for (size_t i = bot->data.list.len - 1; i >= idx; i--)
		bot->data.list.value[i + item_len] = bot->data.list.value[i];
	bot->data.list.len += item_len;

	if (is_list(sec))
		for (size_t i = 0; i < sec->data.list.len; i++)
			bot->data.list.value[idx + i]
				= sec->data.list.value[i];
	else
		bot->data.list.value[idx] = *sec;

	(*len) -= 2;

	return 0;
}

static int
value_eq(struct value *left, struct value *right)
{
	if (left->type != right->type)
		return 0;
	switch (left->type) {
	case BOOL:
		return left->data.bool == right->data.bool;
	case REAL:
		return left->data.real == right->data.real;
	case TEXT:
		return !strcmp(left->data.text, right->data.text);
	case BOOL_L: /* FALLTHROUGH */
	case REAL_L:
	case TEXT_L:
		return 0;
	case BOOL_N: /* FALLTHROUGH */
	case REAL_N:
	case TEXT_N:
		return 1;
	}

	return 0; /* UNREACHABLE */
}

int
value_list_remove(struct value stack[STACK_LEN], size_t *len)
{
	if (*len < 2)
		return -1;
	
	struct value *top = &stack[*len - 1];
	struct value *sec = &stack[*len - 2];
	if (!is_list(sec) || invalid_item(sec, top))
		return -1;
	
	size_t i, j;
	if (is_list(top))
		if (sec->data.list.len < top->data.list.len)
			return -1;
		else
			for (i = 0; i < sec->data.list.len; i++) {
				for (j = 0; j < top->data.list.len; j++)
					if (!value_eq(&sec->data.list.value[i + j],
								  &top->data.list.value[j]))
						break;
				if (j == top->data.list.len) {
					size_t k, k_max = sec->data.list.len - top->data.list.len;
					for (k = i; k < k_max; k++)
						sec->data.list.value[k]
							= sec->data.list.value[k + top->data.list.len];
					sec->data.list.value
						= realloc(sec->data.list.value,
								  (sec->data.list.len) * sizeof(struct value));
					sec->data.list.len -= top->data.list.len;
					return 0;
				}
			}
	else
		for (i = 0; i < sec->data.list.len; i++)
			if (value_eq(&sec->data.list.value[i], top)) {
				for (; i < sec->data.list.len - 1; i++)
					sec->data.list.value[i] = sec->data.list.value[i + 1];
				sec->data.list.value
					= realloc(sec->data.list.value,
							  --sec->data.list.len * sizeof(struct value));
				sec->data.list.len -= top->data.list.len;
				return 0;
			}

	return 0; /* UNREACHABLE */
}

int
value_list_search(struct value stack[STACK_LEN], size_t *len)
{
	if (*len < 2)
		return -1;
	
	struct value *top = &stack[*len - 1];
	struct value *sec = &stack[*len - 2];
	if (!is_list(sec) || invalid_item(sec, top))
		return -1;
	
	size_t i, j;
	if (is_list(top))
		for (i = 0; i < sec->data.list.len; i++) {
			for (j = 0; j < top->data.list.len; j++)
				if (!value_eq(&sec->data.list.value[i + j],
							  &top->data.list.value[j]))
					break;
			if (j == top->data.list.len) {
				*sec = value_real_with((double)i);
				return 0;
			}
		}
	else
		for (i = 0; i < sec->data.list.len; i++)
			if (value_eq(&sec->data.list.value[i], top)) {
				*sec = value_real_with((double)i);
				return 0;
			}

	*sec = value_real_null();

	return 0;
}

int
value_list_getnth(struct value stack[STACK_LEN], size_t *len)
{
	if (*len < 2)
		return -1;

	struct value *top = &stack[*len - 1];
	struct value *sec = &stack[*len - 2];
	if (!is_list(sec) || top->type != REAL)
		return -1;

	size_t idx = (size_t)top->data.real;
	if (idx < sec->data.list.len)
		*sec = sec->data.list.value[idx];
	else
		switch (sec->type) {
		case BOOL_L:
			*sec = value_bool_null();
			break;
		case REAL_L:
			*sec = value_real_null();
			break;
		case TEXT_L:
			*sec = value_text_null();
			break;
		default:
			return -1;
		}

	(*len)--;
	
	return 0;
}

int
value_list_setnth(struct value stack[STACK_LEN], size_t *len)
{
	if (*len < 3)
		return -1;
	
	struct value *top = &stack[*len - 1];
	struct value *sec = &stack[*len - 2];
	struct value *bot = &stack[*len - 3];
	if (!is_list(bot) || invalid_item(bot, sec))
		return -1;
	
	size_t idx = (size_t)top->data.real;
	if (bot->data.list.len <= idx)
		return -1;
	
	value_copy(&bot->data.list.value[idx], *sec);
	(*len) -= 2;

	return 0;
}

int
value_list_rmvnth(struct value stack[STACK_LEN], size_t *len)
{
	if (*len < 2)
		return -1;
	
	struct value *top = &stack[*len - 1];
	struct value *sec = &stack[*len - 2];
	if (!is_list(sec) || top->type != REAL)
		return -1;
	
	size_t idx = (size_t)top->data.real;
	if (idx < sec->data.list.len)
		for (size_t i = idx; i < sec->data.list.len - 1; i++)
			sec->data.list.value[i] = sec->data.list.value[i + 1];
	else
		switch (sec->type) {
		case BOOL_L:
			*sec = value_bool_null();
			break;
		case REAL_L:
			*sec = value_real_null();
			break;
		case TEXT_L:
			*sec = value_text_null();
			break;
		default:
			return -1;
		}

	(*len)--;
	
	return 0;
}

int
value_list_length(struct value stack[STACK_LEN], size_t *len)
{
	if (*len < 1)
		return -1;
	
	struct value *top = &stack[*len - 1];
	if (!is_list(top))
		return -1;
	
	*top = value_real_with((double)top->data.list.len);
	(*len)--;
	return 0;
}

int
value_bool_eq(struct value stack[STACK_LEN], size_t *len)
{
	if (*len < 2)
		return -1;

	struct value *top = &stack[*len - 1];
	struct value *sec = &stack[*len - 2];
	if (top->type != BOOL || sec->type != BOOL)
		return -1;
	
	*sec = value_bool_with(top->data.bool == sec->data.bool);
	(*len)--;
	return 0;
}

int
value_bool_ne(struct value stack[STACK_LEN], size_t *len)
{
	if (*len < 2)
		return -1;

	struct value *top = &stack[*len - 1];
	struct value *sec = &stack[*len - 2];
	if (top->type != BOOL || sec->type != BOOL)
		return -1;
	
	*sec = value_bool_with(top->data.bool != sec->data.bool);
	(*len)--;
	return 0;
}

int
value_real_eq(struct value stack[STACK_LEN], size_t *len)
{
	if (*len < 2)
		return -1;

	struct value *top = &stack[*len - 1];
	struct value *sec = &stack[*len - 2];
	if (top->type != REAL || sec->type != REAL)
		return -1;
	
	*sec = value_bool_with(top->data.real == sec->data.real);
	(*len)--;
	return 0;
}

int
value_real_ne(struct value stack[STACK_LEN], size_t *len)
{
	if (*len < 2)
		return -1;

	struct value *top = &stack[*len - 1];
	struct value *sec = &stack[*len - 2];
	if (top->type != REAL || sec->type != REAL)
		return -1;
	
	*sec = value_bool_with(top->data.real != sec->data.real);
	(*len)--;
	return 0;
}

int
value_real_gt(struct value stack[STACK_LEN], size_t *len)
{
	if (*len < 2)
		return -1;

	struct value *top = &stack[*len - 1];
	struct value *sec = &stack[*len - 2];
	if (top->type != REAL || sec->type != REAL)
		return -1;
	
	*sec = value_bool_with(top->data.real < sec->data.real);
	(*len)--;
	return 0;
}

int
value_real_ge(struct value stack[STACK_LEN], size_t *len)
{
	if (*len < 2)
		return -1;

	struct value *top = &stack[*len - 1];
	struct value *sec = &stack[*len - 2];
	if (top->type != REAL || sec->type != REAL)
		return -1;
	
	*sec = value_bool_with(top->data.real <= sec->data.real);
	(*len)--;
	return 0;
}

int
value_real_lt(struct value stack[STACK_LEN], size_t *len)
{
	if (*len < 2)
		return -1;

	struct value *top = &stack[*len - 1];
	struct value *sec = &stack[*len - 2];
	if (top->type != REAL || sec->type != REAL)
		return -1;
	
	*sec = value_bool_with(top->data.real > sec->data.real);
	(*len)--;
	return 0;
}

int
value_real_le(struct value stack[STACK_LEN], size_t *len)
{
	if (*len < 2)
		return -1;

	struct value *top = &stack[*len - 1];
	struct value *sec = &stack[*len - 2];
	if (top->type != REAL || sec->type != REAL)
		return -1;
	
	*sec = value_bool_with(top->data.real >= sec->data.real);
	(*len)--;
	return 0;
}

int
value_text_eq(struct value stack[STACK_LEN], size_t *len)
{
	if (*len < 2)
		return -1;
	
	struct value *top = &stack[*len - 1];
	struct value *sec = &stack[*len - 2];
	if (top->type != TEXT || sec->type != TEXT)
		return -1;
	
	*sec = value_bool_with(!strcmp(top->data.text, sec->data.text));
	(*len)--;
	return 0;
}

int
value_text_ne(struct value stack[STACK_LEN], size_t *len)
{
	if (*len < 2)
		return -1;
	
	struct value *top = &stack[*len - 1];
	struct value *sec = &stack[*len - 2];
	if (top->type != TEXT || sec->type != TEXT)
		return -1;
	
	*sec = value_bool_with(strcmp(top->data.text, sec->data.text));
	(*len)--;
	return 0;
}

int
value_list_eq(struct value stack[STACK_LEN], size_t *len)
{
	if (*len < 2)
		return -1;
	
	struct value *top = &stack[*len - 1];
	struct value *sec = &stack[*len - 2];
	if (top->type != sec->type || !is_list(top) || !is_list(sec))
		return -1;
	if (top->data.list.len != sec->data.list.len) {
		*sec = value_bool_with(0);
		goto end;
	}
	else
		for (size_t i = 0; i < top->data.list.len; i++)
			if (!value_eq(&top->data.list.value[i],
						  &sec->data.list.value[i])) {
				*sec = value_bool_with(0);
				goto end;
			}
	
	*sec = value_bool_with(1);
	end:
	(*len)--;
	return 0;
}

int
value_list_ne(struct value stack[STACK_LEN], size_t *len)
{
	if (*len < 2)
		return -1;
	
	value_list_eq(stack, len);
	stack[0].data.bool = !stack[0].data.bool;

	return 0;
}

int
value_to_bool(struct value stack[STACK_LEN], size_t *len)
{
	if (*len < 1)
		return -1;

	struct value *top = &stack[*len - 1];
	switch (top->type) {
	case BOOL_N: /* FALLTHROUGHT */
	case REAL_N:
	case TEXT_N:
		*top = value_bool_null();
		break;
	case BOOL:
		break;
	case REAL:
		*top = value_bool_with((int)top->data.real);
		break;
	case TEXT:
		if (!strcmp(top->data.text, "true"))
			*top = value_bool_with(1);
		else if (!strcmp(top->data.text, "false"))
			*top = value_bool_with(0);
		else
			return -1;
		break;
	default:
		return -1;
	}

	return 0;
}

int
value_to_real(struct value stack[STACK_LEN], size_t *len)
{
	if (*len < 1)
		return -1;

	struct value *top = &stack[*len - 1];
	switch (top->type) {
	case BOOL_N: /* FALLTHROUGHT */
	case REAL_N:
	case TEXT_N:
		*top = value_real_null();
		break;
	case BOOL:
		*top = value_real_with(top->data.bool);
		break;
	case REAL:
		break;
	case TEXT:
		*top = value_real_with(atof(top->data.text));
		break;
	default:
		return -1;
	}
	
	return 0;
}

int
value_to_text(struct value stack[STACK_LEN], size_t *len)
{
	if (*len < 1)
		return -1;

	struct value *top = &stack[*len - 1];
	switch (top->type) {
	case BOOL_N: /* FALLTHROUGHT */
	case REAL_N:
	case TEXT_N:
		*top = value_text_null();
		break;
	case BOOL:
		*top = value_text_with(top->data.bool ? "true" : "false");
		break;
	case REAL:
		char buf[50];
		snprintf(buf, 50, "%lf", top->data.real);
		for (size_t i = BUF_LEN - 1; i > 0; i--) {
			if ('0' < buf[i] && buf[i] <= '9') {
				buf[i + 1] = '\0';
				break;
			} else if (buf[i] == '.') {
				buf[i] = '\0';
				break;
			}
		}
		*top = value_text_with(buf);
		break;
	case TEXT:
		break;
	default:
		return -1;
	}
	
	return 0;
}

#ifdef _WIN32
#include <windows.h>
#include <time.h>
#endif
#define RAND_BUF_LEN 4
#define RAND_BUF_SIZE BUF_LEN * sizeof(uint64_t)

static uint64_t buf[RAND_BUF_LEN];

void
init_rand_buf()
{
#ifdef _WIN32
	if (!CryptGenRandom(GetModuleHandle(NULL), BUF_SIZE, buf)) {
		srand(time(NULL));
		uint8_t bufbuf[RAND_BUF_SIZE];
		for (size_t i = 0; i < RAND_BUF_SIZE; i++)
			bufbuf[i] = rand();
		memcpy(buf, bufbuf, RAND_BUF_SIZE);
	}
#else
	arc4random_buf(buf, RAND_BUF_SIZE);
#endif
}

static uint64_t
rotl(const uint64_t x, int k)
{
	return (x << k) | (x >> (64 - k));
}

static uint64_t
xoshiro256ss()
{
	const uint64_t result = rotl(buf[1] * 5, 7) * 9;
	const uint64_t tmp = buf[1] << 17;

	buf[2] ^= buf[0];
	buf[3] ^= buf[1];
	buf[1] ^= buf[2];
	buf[0] ^= buf[3];
	buf[2] ^= tmp;

	buf[3] = rotl(buf[3], 45);

	return result;
}

int
value_bool_rand(struct value stack[STACK_LEN], size_t *len)
{
	if (*len == STACK_LEN)
		return -1;
	
	stack[(*len)++] = value_bool_with(xoshiro256ss() % 2);
	return 0;
}

int
value_real_rand(struct value stack[STACK_LEN], size_t *len)
{
	if (*len == STACK_LEN)
		return -1;
	
	stack[(*len)++] = value_real_with((double)xoshiro256ss());
	return 0;
}

int
value_text_rand(struct value stack[STACK_LEN], size_t *len)
{
	if (*len == STACK_LEN)
		return -1;
	
	char txt[] = " ";
	txt[0] = xoshiro256ss() % 95 + 32; /* ASCII 32 ~ 126 */
	stack[(*len)++] = value_text_with(txt);
	return 0;
}
