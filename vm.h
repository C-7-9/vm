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

#include "func.h"
#define CALL_MAX 1024

struct vm {
	struct value stack[STACK_LEN];
	size_t stack_len;

	struct value *var;
	size_t var_len;
	
	struct value *con;
	size_t con_len;

	size_t *func;
	size_t func_len;

	uint16_t call[CALL_MAX];
	size_t call_len;

	uint8_t *bc;
	size_t bc_len;
	
	size_t pc;
};

int vm_init(struct vm *, uint8_t *, size_t);
int vm_run_all(struct vm *);
int vm_run_one(struct vm *);
