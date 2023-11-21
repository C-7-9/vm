#include "value.h"
#include "str.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define TRUE 0xff
#define FALSE 0

struct value
value_bool_null()
{
	struct value val;
	val.type = BOOL_N;
	val.data.bool = 0;
	return val;
}

struct value
value_real_null()
{
	struct value val;
	val.type = REAL_N;
	val.data.real = 0;
	return val;
}

struct value
value_text_null()
{
	struct value val;
	val.type = TEXT_N;
	val.data.text = str_new("");
	return val;
}

struct value
value_bool_with(const int n)
{
	struct value val;
	val.type = BOOL;
	val.data.bool = n ? TRUE : FALSE;
	return val;
}

struct value
value_real_with(const double num)
{
	struct value val;
	val.type = REAL;
	val.data.real = num;
	return val;
}

struct value
value_text_with(const char *str)
{
	struct value val;
	val.type = TEXT;
	val.data.text = str_new(str);
	return val;
}

void
value_free(struct value *val)
{
	switch (val->type) {
	case TEXT:
		free(val->data.text);
		break;
	case BOOL_L: /* FALLTHROUGH */
	case REAL_L:
		free(val->data.list.value);
		break;
	case TEXT_L:
		for (size_t i = 0; i < val->data.list.len; i++)
			free(val->data.list.value[i].data.text);
		break;
	default:
	}
}

void
value_copy(struct value *left, const struct value right)
{
	value_free(left);
	left->type = right.type;
	switch (right.type) {
	case TEXT:
		left->data.text = str_new(right.data.text);
		break;
	case BOOL_L: /* FALLTHROUGH */
	case REAL_L: {
		left->data.list.len = right.data.list.len;
		size_t right_size = right.data.list.len * sizeof(struct value);
		left->data.list.value = malloc(right_size);
		memcpy(left->data.list.value, right.data.list.value, right_size);
		break;
	}
	case TEXT_L: {
		left->data.list.len = right.data.list.len;
		size_t right_size = right.data.list.len * sizeof(struct value);
		left->data.list.value = malloc(right_size);
		for (size_t i = 0; i < right.data.list.len; i++) {
			left->data.list.value[i].type = TEXT;
			left->data.list.value[i].data.text 
				= str_new(right.data.list.value[i].data.text);
		}
		break;
	}
	case BOOL: /* FALLTHROUGH */
	case REAL:
	case BOOL_N:
	case REAL_N:
	case TEXT_N:
		left->data = right.data;
		break;
	}
}

char *
value_into_text(const struct value val)
{
	switch (val.type) {
	case BOOL:
		return val.data.bool ? str_new("true") : str_new("false");
	case REAL:
		char buf[50];
		snprintf(buf, 50, "%lf", val.data.real);
		return str_new(buf);
	case TEXT: {
		char *text = str_new("");
		str_push(&text, '"');
		str_push_str(&text, val.data.text);
		str_push(&text, '"');
		return text;
	}
	case BOOL_N:
		return str_new("BOOL_N");
	case REAL_N:
		return str_new("REAL_N");
	case TEXT_N:
		return str_new("TEXT_N");
	case BOOL_L: {
		char *text = str_new("[");
		for (size_t i = 0; i < val.data.list.len; i++)
			if (val.data.list.value[i].type != BOOL)
				return NULL;
			else {
				str_push_str(&text, value_into_text(val.data.list.value[i]));
				str_push(&text, ',');
			}
		text[strlen(text) - 1] = ']';
		return text;
	}
	case REAL_L: {
		char *text = str_new("[");
		for (size_t i = 0; i < val.data.list.len; i++)
			if (val.data.list.value[i].type != REAL)
				return NULL;
			else {
				str_push_str(&text, value_into_text(val.data.list.value[i]));
				str_push(&text, ',');
			}
		text[strlen(text) - 1] = ']';
		return text;
	}
	case TEXT_L: {
		char *text = str_new("[");
		for (size_t i = 0; i < val.data.list.len; i++)
			if (val.data.list.value[i].type != TEXT)
				return NULL;
			else {
				str_push_str(&text, value_into_text(val.data.list.value[i]));
				str_push(&text, ',');
			}
		text[strlen(text) - 1] = ']';
		return text;
	}
	}

	return NULL; /* UNREACHABLE */
}

struct value
value_from_text(char *text)
{
}
