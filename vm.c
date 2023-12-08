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
	if (!vm->stack_len)
		return -1;
	
	if (index > vm->var_len)
		return -1;
	else if (index < vm->var_len) {
		if (vm->var[index].type != vm->stack[vm->stack_len - 1].type)
			return -1;
	} else
		vm->var = realloc(vm->var, ++vm->var_len * sizeof(struct value));
	
	value_copy(&vm->var[index], vm->stack[vm->stack_len - 1]);
	value_free(&vm->stack[vm->stack_len - 1]);
	vm->stack_len--;

	return 0;
}

static int
call_push(struct vm *vm, struct call_info info)
{
	if (vm->call_len == CALL_MAX)
		return -1;
	vm->call_info[vm->call_len++] = info;
	return 0;
}

static int
call_pop(struct vm *vm, struct call_info *info)
{
	if (!vm->call_len)
		return -1;
	*info = vm->call_info[--vm->call_len];
	return 0;
}

static int
call_externel_fun(struct vm *vm, ssize_t fun_num)
{
	if (fun_num < 0)
		return 0;
	
	switch (fun_num) {
	/* FILLME */
	default:
		return -1;
	}

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
	uint8_t opcode = vm->bc[vm->pc] & 0xf0;
	uint16_t oprand = (((uint16_t)vm->bc[vm->pc] << 8) & 0x0f00)
				      | vm->bc[vm->pc + 1];

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
		size_t new_len = vm->var_len + oprand;
		vm->var = realloc(vm->var, new_len * sizeof(struct value));
		while (vm->var_len < new_len)
			value_copy(&vm->var[vm->var_len++], vm->stack[--vm->stack_len]);
		break;
	case 0x50: /* call user-defined function */	
		if (oprand >= vm->fun_len)
			return -1;
		if (vm->bc[vm->fun[oprand]] != 0x40)
			return -1;
		struct call_info new_info = { vm->pc, vm->var_len };
		if (call_push(vm, new_info))
			return -1;
		vm->pc = vm->fun[oprand];
		return 0;
	case 0x60: /* call built-in function */
		if (call_external_fun(vm, (ssize_t)oprand - (ssize_t)vm->fun_len))
			return -1;
		if (fun[oprand](vm->stack, &vm->stack_len))
			return -1;
		break;
	case 0x70: /* return */
		vm->stack_len = 1;
		vm->stack[0] = vm->stack[vm->stack_len - 1];
		/* FALLTHROUGH */
	case 0x80: /* tail of function */
		struct call_info top_info;
		if (call_pop(vm, &top_info))
			return -1;
		vm->pc = top_info.pc;
		vm->var = realloc(vm->var, top_info.var_len * sizeof(struct value));
		vm->var_len = top_info.var_len;
		break;
	case 0x90: /* loop head */
		struct flow_info new_loop;
		new_loop.head = vm->pc;
		new_loop.tail = vm->pc + (oprand * 2);
		new_loop.var_len = vm->var_len;
		vm->flow_info[vm->flow_len++] = new_loop;
		break;
	case 0xA0: /* loop next */
		vm->pc = vm->flow_info[vm->flow_len - 1].tail - 2;
		break;
	case 0xB0: /* loop exit */
		vm->pc = vm->flow_info[vm->flow_len - 1].tail;
		break;
	case 0xC0: /* loop tail */
		struct flow_info last_loop = vm->flow_info[vm->flow_len - 1];
		vm->pc = last_loop.head;
		vm->var_len = last_loop.var_len;
		vm->var = realloc(vm->var, vm->var_len * sizeof(struct value));
		break;
	case 0xD0: /* when */
		if (!vm->stack_len)
			return -1;
		struct value top = vm->stack[--vm->stack_len];
		if (top.type != BOOL)
			return -1;
		if (!top.data.bool)
			vm->pc += (oprand + 1) * 2;
		struct flow_info new_if;
		new_if.var_len = vm->var_len;
		vm->flow_info[vm->flow_len++] = new_if;
		break;
	case 0xE0: /* else */
		vm->pc += oprand * 2;
		break;
	case 0xF0: /* when tail */
		struct flow_info last_if = vm->flow_info[--vm->flow_len];
		vm->var_len = last_if.var_len;
		vm->var = realloc(vm->var, vm->var_len * sizeof(struct value));
		break;
	}

	vm->pc += 2;

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
