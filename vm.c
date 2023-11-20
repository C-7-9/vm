#include "vm.h"
#include <stdlib.h>

static int
stack_load(struct vm *vm, size_t index)
{
	if (vm->var_len < index + 1)
		return -1;
	if (!vm->stack_len)
		return -1;
	
	if (vm->var_len == index + 1)
		vm->var = realloc(vm->var, ++vm->var_len * sizeof(struct value));
	value_copy(&vm->var[index], vm->stack[vm->stack_len - 1]);
	value_free(&vm->stack[vm->stack_len - 1]);
	vm->stack_len--;

	return 0;
}

int
vm_init(struct vm *vm)
{
	/* TODO */

	init_rand_buf();
	return 0;
}

int
vm_run_one(struct vm *vm)
{
	size_t pc = vm->pc++ * 2; 
	size_t opcode = vm->bc[pc] & 0xf0;
	size_t oprand = ((size_t)(vm->bc[pc] & 0x0f) << 8) 
					| vm->bc[pc + 1];

	switch (opcode) {
	case 0x00: /* shutdown */
		return 1;
	case 0x10: /* push constant into stack */
		if (vm->stack_len == STACK_LEN)
			return -1;
		value_copy(&vm->stack[vm->stack_len++], vm->con[oprand]);
		break;
	case 0x20: /* push variable into stack */
		if (vm->stack_len == STACK_LEN)
			return -1;
		value_copy(&vm->stack[vm->stack_len++], vm->var[oprand]);
		break;
	case 0x30: /* load variable into memory */
		if (stack_load(vm, oprand))
			return -1;
		break;
	case 0x40: /* head of function */
		if (oprand > vm->stack_len)
			return -1;
		for (size_t i = 0; i < oprand; i++)
			if (stack_load(vm, vm->var_len++))
				return -1;
		break;
	case 0x50: /* call user-defined function */
	case 0x60: /* call built-in function */
		if (func[oprand](vm->stack, &vm->stack_len))
			return -1;
		break;
	case 0x70: /* return */
	case 0x80: /* tail of function */
	case 0x90: /* loop start */
	case 0xA0: /* loop next */
	case 0xB0: /* loop exit */
	case 0xC0: /* loop end */
	case 0xD0: /* if */
	case 0xE0: /* or */
	case 0xF0: /* ifor end */
	}

	return 0;
}

int
vm_run_all(struct vm *vm)
{
	while (vm->pc * 2 < vm->bc_len) {
		int result = vm_run_one(vm);
		if (result < 0)
			return -1;
		else if (result > 0)
			return 0;
	}

	return 0;
}
