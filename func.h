#include "value.h"

#define STACK_LEN 32

int value_bool_and(struct value[STACK_LEN], size_t *);
int value_bool_oor(struct value[STACK_LEN], size_t *);
int value_bool_xor(struct value[STACK_LEN], size_t *);
int value_bool_not(struct value[STACK_LEN], size_t *);

int value_real_add(struct value[STACK_LEN], size_t *);
int value_real_sub(struct value[STACK_LEN], size_t *);
int value_real_mul(struct value[STACK_LEN], size_t *);
int value_real_div(struct value[STACK_LEN], size_t *);
int value_real_rem(struct value[STACK_LEN], size_t *);

int value_text_append(struct value[STACK_LEN], size_t *);
int value_text_insert(struct value[STACK_LEN], size_t *);
int value_text_remove(struct value[STACK_LEN], size_t *);
int value_text_search(struct value[STACK_LEN], size_t *);
int value_text_getnth(struct value[STACK_LEN], size_t *);
int value_text_setnth(struct value[STACK_LEN], size_t *);
int value_text_rmvnth(struct value[STACK_LEN], size_t *);
int value_text_length(struct value[STACK_LEN], size_t *);

int value_list_append(struct value[STACK_LEN], size_t *);
int value_list_insert(struct value[STACK_LEN], size_t *);
int value_list_remove(struct value[STACK_LEN], size_t *);
int value_list_search(struct value[STACK_LEN], size_t *);
int value_list_getnth(struct value[STACK_LEN], size_t *);
int value_list_setnth(struct value[STACK_LEN], size_t *);
int value_list_rmvnth(struct value[STACK_LEN], size_t *);
int value_list_length(struct value[STACK_LEN], size_t *);

int value_bool_eq(struct value[STACK_LEN], size_t *);
int value_bool_ne(struct value[STACK_LEN], size_t *);

int value_real_eq(struct value[STACK_LEN], size_t *);
int value_real_ne(struct value[STACK_LEN], size_t *);
int value_real_gt(struct value[STACK_LEN], size_t *);
int value_real_ge(struct value[STACK_LEN], size_t *);
int value_real_lt(struct value[STACK_LEN], size_t *);
int value_real_le(struct value[STACK_LEN], size_t *);

int value_text_eq(struct value[STACK_LEN], size_t *);
int value_text_ne(struct value[STACK_LEN], size_t *);

int value_list_eq(struct value[STACK_LEN], size_t *);
int value_list_ne(struct value[STACK_LEN], size_t *);

int value_to_bool(struct value[STACK_LEN], size_t *);
int value_to_real(struct value[STACK_LEN], size_t *);
int value_to_text(struct value[STACK_LEN], size_t *);

int value_bool_rand(struct value[STACK_LEN], size_t *);
int value_real_rand(struct value[STACK_LEN], size_t *);
int value_text_rand(struct value[STACK_LEN], size_t *);

void init_rand_buf();

int (*func[])(struct value[STACK_LEN], size_t *) = {
	value_bool_and, 
	value_bool_oor,
	value_bool_xor,
	value_bool_not,
	value_real_add,
	value_real_sub,
	value_real_mul,
	value_real_div,
	value_real_rem,
	value_text_append, 
	value_text_insert,
	value_text_remove,
	value_text_search,
	value_text_getnth,
	value_text_setnth,
	value_text_rmvnth,
	value_list_append,
	value_list_insert,
	value_list_remove,
	value_list_search,
	value_list_getnth,
	value_list_setnth,
	value_list_rmvnth,
	value_bool_eq, 
	value_bool_ne,
	value_real_eq,
	value_real_ne,
	value_real_gt,
	value_real_ge,
	value_real_lt,
	value_real_le,
	value_text_eq,
	value_text_ne,
	value_list_eq,
	value_list_ne,
	value_to_bool, 
	value_to_real,
	value_to_text,
	value_bool_rand, 
	value_real_rand,
	value_text_rand
};
