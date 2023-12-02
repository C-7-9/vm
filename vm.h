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

#include "fun.h"
#define CALL_MAX 1024
#define FLOW_MAX 8

struct flow_info {
	size_t head;
	size_t tail;
	size_t var_len;
};

struct call_info {
	size_t pc;
	size_t var_len;
};

struct vm {
	struct value stack[STACK_LEN];
	size_t stack_len;

	struct value *var;
	size_t var_len;
	
	struct value *con;
	size_t con_len;

	size_t *fun;
	size_t fun_len;

	struct call_info call_info[CALL_MAX];
	size_t call_len;

	struct flow_info flow_info[FLOW_MAX];
	size_t flow_len;

	uint8_t *bc;
	size_t bc_len;
	
	size_t pc;
};

int vm_init(struct vm *, uint8_t *, size_t);
int vm_run_all(struct vm *);
int vm_run_one(struct vm *);
