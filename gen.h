#include "vm.h"

struct vm gen_new_vm();
void gen_add_bc(struct vm *, uint16_t);
void gen_add_con(struct vm *, struct value);
void gen_add_fun(struct vm *, size_t);
