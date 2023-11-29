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

#include "vm.h"
#include <stdlib.h>

static int (*fun[])(struct value[STACK_LEN], size_t *) = {
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

static int
stack_push(struct vm *vm, struct value val)
{
	if (vm->stack_len == STACK_LEN)
		return -1;
	value_copy(&vm->stack[vm->stack_len++], val);
	return 0;
}

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

static int
call_push(struct vm *vm, uint16_t fun_num)
{
	if (vm->call_len == CALL_MAX)
		return -1;
	vm->call[vm->call_len++] = fun_num;
	return 0;
}

static int
call_pop(struct vm *vm, uint16_t *fun_num)
{
	if (!vm->call_len)
		return -1;
	*fun_num = vm->call[--vm->call_len];
	return 0;
}

int
vm_init(struct vm *vm, uint8_t *bytes, size_t bytes_len)
{
	/* TODO */

	init_rand_buf();
	return 0;
}

int
vm_run_one(struct vm *vm)
{
	size_t pc = vm->pc++ * 2; 
	uint8_t opcode = vm->bc[pc] & 0xf0;
	uint16_t oprand = ((uint16_t)(vm->bc[pc] & 0x0f) << 8) 
					| vm->bc[pc + 1];

	switch (opcode) {
	case 0x00: /* shutdown */
		return 1;
	case 0x10: /* push constant into stack */
		if (stack_push(vm, vm->con[oprand]))
			return -1;
		break;
	case 0x20: /* push variable into stack */
		if (stack_push(vm, vm->var[oprand]))
			return -1;
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
		if (oprand >= vm->fun_len)
			return -1;
		vm->pc = vm->fun[oprand];
		if ((vm->bc[vm->pc * 2] & 0xf0) != 0x50)
			return -1;
		call_push(vm, oprand);
		break;
	case 0x60: /* call built-in function */
		if (fun[oprand](vm->stack, &vm->stack_len))
			return -1;
		break;
	case 0x70: /* return */ {
		uint16_t fun_num;
		if (call_pop(vm, &fun_num))
			return -1;
		vm->pc = vm->fun[fun_num] * 2;
		stack_push(vm, vm->var[oprand]);
		break;
	}
	case 0x80: /* tail of function */ {
		uint16_t fun_num;
		if (call_pop(vm, &fun_num))
			return 1; /* end of program */
		vm->pc = vm->fun[fun_num] * 2;
		break;
	}
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
	for (;;) {
		int result = vm_run_one(vm);
		if (result < 0)
			return -1;
		else if (result > 0)
			return 0;
	}

	return 0; /* UNREACHABLE */
}
