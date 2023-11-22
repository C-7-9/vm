#include "value.h"
#include "str.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define TRUE 0xff
#define FALSE 0
#define BUF_LEN 50

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
		left->type = right.type;
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
		left->type = right.type;
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
		char buf[BUF_LEN] = { 0 };
		sprintf(buf, "%lf", val.data.real);
		for (size_t i = BUF_LEN - 1; i > 0; i--) {
			if ('0' < buf[i] && buf[i] <= '9') {
				buf[i + 1] = '\0';
				break;
			} else if (buf[i] == '.') {
				buf[i] = '\0';
				break;
			}
		}
		return str_new(buf);
	case TEXT: {
		char *text = str_new("");
		str_push(&text, '"');
		str_push_str(&text, val.data.text);
		str_push(&text, '"');
		return text;
	}
	case BOOL_N:
		return str_new("BOOL");
	case REAL_N:
		return str_new("REAL");
	case TEXT_N:
		return str_new("TEXT");
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

static struct value
value_text_from_raw_text(char *text)
{
	text[strlen(text) - 1] = '\0';
	return value_text_with(text + 1);
}

static int
value_list_from_raw_text(struct value *res_list, char *text)
{
	struct value val, *list = malloc(0);
	size_t list_len = 0;
	text[strlen(text) - 1] = '\0';
	char *tok = strtok(text + 1, ",");
	while (tok != NULL) {
		value_from_text(&val, tok);
		list = realloc(list, ++list_len * sizeof(struct value));
		list[list_len - 1] = val;

		tok = strtok(NULL, ",");
	}

	switch (list[0].type) {
	case BOOL:
		res_list->type = BOOL_L;
		break;
	case REAL:
		res_list->type = REAL_L;
		break;
	case TEXT:
		res_list->type = TEXT_L;
		break;
	default:
		return -1;
	}
	res_list->data.list.value = list;
	res_list->data.list.len = list_len;

	return 0;
}

int
value_from_text(struct value *val, char *text)
{
	if (!strlen(text))
		return -1;
	if (!strcmp(text, "BOOL"))
		*val = value_bool_null();
	else if (!strcmp(text, "REAL"))
		*val = value_real_null();
	else if (!strcmp(text, "TEXT"))
		*val = value_text_null();
	else if (!strcmp(text, "true"))
		*val = value_bool_with(1);
	else if (!strcmp(text, "false"))
		*val = value_bool_with(0);
	else if (text[0] == '"')
		*val = value_text_from_raw_text(text);
	else if (('0' <= text[0] && text[0] <= '9') || text[0] == '.')
		*val = value_real_with(atof(text));
	else if (text[0] == '[')
		value_list_from_raw_text(val, text);
	else
		return -1;

	return 0;
}
