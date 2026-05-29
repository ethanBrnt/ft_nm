/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eburnet <eburnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 15:39:13 by eburnet           #+#    #+#             */
/*   Updated: 2026/04/07 10:43:15 by eburnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <elf.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>

extern uint16_t e_type;

void quick_sort_32(Elf32_Sym *sym_table, int *sym_tab_index, int start, int end, char *str_table);
void quick_sort_64(Elf64_Sym *sym_table, int *sym_tab_index, int start, int end, char *str_table);
char find_sym_type_64(Elf64_Sym sym_tab_elem, Elf64_Shdr *s_head_first);
char find_sym_type_32(Elf32_Sym sym_tab_elem, Elf32_Shdr *s_head_first);
char *padding_management(int st_value, char type, bool is_32);
void *parsing_mmaping(int argc, char *argv[]);
