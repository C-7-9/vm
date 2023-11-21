#include "func.h"
#define CALL_LEN 1024

struct vm {
	struct value stack[STACK_LEN];
	size_t stack_len;

	struct value *var;
	size_t var_len;
	
	struct value *con;
	size_t con_len;

	size_t *func;
	size_t func_len;

	uint16_t call[CALL_LEN];
	size_t call_len;

	uint8_t *bc;
	size_t bc_len;
	
	size_t pc;
};

int vm_init(struct vm *);
int vm_run_all(struct vm *);
int vm_run_one(struct vm *);
