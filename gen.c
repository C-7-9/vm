#include "gen.h"
#include <stdlib.h>

struct vm
gen_new_vm()
{
	struct vm vm;
	vm.con = malloc(0);
	vm.var = malloc(sizeof(struct value)); /* dump memory */
	vm.fun = malloc(0);
	vm.bc = malloc(0);
	vm.stack_len = 0;
	vm.con_len = 0;
	vm.var_len = 1;
	vm.fun_len = 0;
	vm.bc_len = 0;
	vm.call_len = 0;
	vm.pc = 0;
	return vm;
}

void
gen_add_bc(struct vm *vm, uint16_t code)
{
	vm->bc_len += 2;
	vm->bc = realloc(vm->bc, vm->bc_len);

	if ((uint8_t)0x00ff) { 	/* Big Endian */
		vm->bc[vm->bc_len - 2] = (uint8_t)(code >> 8);
		vm->bc[vm->bc_len - 1] = (uint8_t)code;
	} else {				/* Little Endian */
		vm->bc[vm->bc_len - 2] = (uint8_t)code;
		vm->bc[vm->bc_len - 1] = (uint8_t)(code >> 8);
	}
}

void
gen_add_con(struct vm *vm, struct value con)
{
	vm->con = realloc(vm->con, ++vm->con_len * sizeof(struct value));
	vm->con[vm->con_len - 1] = con;
}

void
gen_add_fun(struct vm *vm, size_t fun)
{
	vm->fun = realloc(vm->fun, ++vm->fun_len * sizeof(size_t));
	vm->fun[vm->fun_len - 1] = fun * 2;
}
