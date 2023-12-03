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

#include <stddef.h>

char *str_empty();
char *str_new(const char *);
int str_replace(char **, char **);
int str_get_index(char **, char *, size_t);
int str_set_index(char **, char, size_t);
int str_insert(char **, char, size_t);
int str_insert_str(char **, char *, size_t);
int str_remove(char **, char *, size_t);
int str_remove_str(char **, char *);
int str_push(char **, char);
int str_push_str(char **, char *);
int str_pop(char **, char *);
int str_pop_str(char **, char **, size_t);
