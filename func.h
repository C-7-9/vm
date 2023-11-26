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

