/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eburnet <eburnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/12 11:51:06 by eburnet           #+#    #+#             */
/*   Updated: 2026/05/29 20:44:20 by eburnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"
uint16_t e_type;

int sort_print_data_32(void *ptr_e_stroff, int symbol_nbr, void *ptr_e_symoff, Elf32_Shdr *s_head_first )
{
	Elf32_Sym *sym_table = (Elf32_Sym *)ptr_e_symoff;
	char *str_table = (char *)ptr_e_stroff;
	int *sym_tab_index = malloc(sizeof(int) * symbol_nbr);
	if (!sym_tab_index)
		return (1);
	int printable_sym_nbr = 0;
	for (int i = 0; i < symbol_nbr; i++)
	{
		if (sym_table[i].st_name != 0)
		{
			sym_tab_index[printable_sym_nbr] = i;
			printable_sym_nbr++;
		}
	}
	quick_sort_32(sym_table, sym_tab_index, 0, printable_sym_nbr, str_table);
	for (int i = 0; i <= printable_sym_nbr; i++)
	{
		char *name = str_table + sym_table[sym_tab_index[i]].st_name;
		if (!name || ft_strlen(name) == 0)
			continue;
		char type = find_sym_type_32(sym_table[sym_tab_index[i]], s_head_first);
		if (type == 'e')
			continue;
		char *padding = padding_management(sym_table[sym_tab_index[i]].st_value, type, true);
		if (!padding)
			return (free(sym_tab_index), 1);
		if (type == 'U' || type == 'w')
			ft_printf("%s %c %s\n", padding, type, name);
		else
			ft_printf("%s%x %c %s\n", padding, sym_table[sym_tab_index[i]].st_value, type, name);
		free(padding);
	}
	free(sym_tab_index);
	return (0);
}

int sort_print_data_64(void *ptr_e_stroff, int symbol_nbr, void *ptr_e_symoff, Elf64_Shdr *s_head_first )
{
	Elf64_Sym *sym_table = (Elf64_Sym *)ptr_e_symoff;
	char *str_table = (char *)ptr_e_stroff;
	int *sym_tab_index = malloc(sizeof(int) * symbol_nbr);
	if (!sym_tab_index)
		return (1);
	int printable_sym_nbr = 0;
	for (int i = 0; i < symbol_nbr; i++)
	{
		if (sym_table[i].st_name != 0)
		{
			sym_tab_index[printable_sym_nbr] = i;
			printable_sym_nbr++;
		}
	}
	quick_sort_64(sym_table, sym_tab_index, 0, printable_sym_nbr, str_table);
	for (int i = 0; i <= printable_sym_nbr; i++)
	{
		char *name = str_table + sym_table[sym_tab_index[i]].st_name;
		if (!name || ft_strlen(name) == 0)
			continue;
		char type = find_sym_type_64(sym_table[sym_tab_index[i]], s_head_first);
		if (type == 'e')
			continue;
		char *padding = padding_management(sym_table[sym_tab_index[i]].st_value, type, false);
		if (!padding)
			return (free(sym_tab_index), 1);
		if (type == 'U' || type == 'w')
			ft_printf("%s %c %s\n", padding, type, name);
		else
			ft_printf("%s%x %c %s\n", padding, sym_table[sym_tab_index[i]].st_value, type, name);
		free(padding);
	}
	free(sym_tab_index);
	return (0);
}

int find_sym_str_tab_32(void *ptr)
{
	Elf32_Ehdr *e_head = (Elf32_Ehdr *)ptr;
	if (e_head->e_ident[EI_MAG0] != ELFMAG0 || e_head->e_ident[EI_MAG1] != ELFMAG1 || e_head->e_ident[EI_MAG2] != ELFMAG2 || e_head->e_ident[EI_MAG3] != ELFMAG3)
		return (ft_putstr_fd("ft_nm: file format not recognized\n", 2), 1);
	if (e_head->e_shstrndx != e_head->e_shnum - 1)
		return (ft_putstr_fd("ft_nm: file format not recognized\n", 2), 1);
	void *ptr_e_shoff = (char *)ptr + e_head->e_shoff;
	int section_nbr = e_head->e_shnum;
	Elf32_Shdr *s_head_first = (Elf32_Shdr *)ptr_e_shoff;
	Elf32_Shdr *s_head_sym;
	Elf32_Shdr *s_head_str;
	for (int i = 0; i < section_nbr; i++)
	{
		Elf32_Shdr *s_head = (Elf32_Shdr *)ptr_e_shoff;
		if (s_head && s_head->sh_type && (s_head->sh_type == SHT_SYMTAB || s_head->sh_type == SHT_DYNSYM))
			s_head_sym = s_head;
		else if (s_head && s_head->sh_type && s_head->sh_type == SHT_STRTAB && s_head != s_head_first + e_head->e_shstrndx)
			s_head_str = s_head;
		ptr_e_shoff = ptr_e_shoff + sizeof(Elf32_Shdr);
	}
	if (!s_head_sym || !s_head_str)
		return (ft_putstr_fd("ft_nm: SymTab|StrTab not found\n", 2), 1);
	if (s_head_sym->sh_entsize <= 0)
		return (ft_putstr_fd("ft_nm: file format not recognized\n", 2), 1);
	int symbol_nbr = s_head_sym->sh_size / s_head_sym->sh_entsize;
	void *ptr_e_symoff = (char *)ptr + s_head_sym->sh_offset;
	void *ptr_e_stroff = (char *)ptr + s_head_str->sh_offset;
	if (sort_print_data_32(ptr_e_stroff, symbol_nbr, ptr_e_symoff, s_head_first) == 1)
		return (1);
	else
		return (0);
}

int find_sym_str_tab_64(void *ptr)
{
	Elf64_Ehdr *e_head = (Elf64_Ehdr *)ptr;
	if (e_head->e_ident[EI_MAG0] != ELFMAG0 || e_head->e_ident[EI_MAG1] != ELFMAG1 || e_head->e_ident[EI_MAG2] != ELFMAG2 || e_head->e_ident[EI_MAG3] != ELFMAG3)
		return (ft_putstr_fd("ft_nm: file format not recognized\n", 2), 1);
	if (e_head->e_shstrndx != e_head->e_shnum - 1)
		return (ft_putstr_fd("ft_nm: file format not recognized\n", 2), 1);
	void *ptr_e_shoff = (char *)ptr + e_head->e_shoff;
	int section_nbr = e_head->e_shnum;
	Elf64_Shdr *s_head_first = (Elf64_Shdr *)ptr_e_shoff;
	Elf64_Shdr *s_head_sym;
	Elf64_Shdr *s_head_str;
	for (int i = 0; i < section_nbr; i++)
	{
		Elf64_Shdr *s_head = (Elf64_Shdr *)ptr_e_shoff;
		if (s_head && s_head->sh_type && (s_head->sh_type == SHT_SYMTAB || s_head->sh_type == SHT_DYNSYM))
			s_head_sym = s_head;
		else if (s_head && s_head->sh_type && s_head->sh_type == SHT_STRTAB && s_head != s_head_first + e_head->e_shstrndx)
			s_head_str = s_head;
		ptr_e_shoff = ptr_e_shoff + sizeof(Elf64_Shdr);
	}
	if (!s_head_sym || !s_head_str)
		return (ft_putstr_fd("ft_nm: SymTab|StrTab not found\n", 2), 1);
	if (s_head_sym->sh_entsize <= 0)
		return (ft_putstr_fd("ft_nm: file format not recognized\n", 2), 1);
	int symbol_nbr = s_head_sym->sh_size / s_head_sym->sh_entsize;
	void *ptr_e_symoff = (char *)ptr + s_head_sym->sh_offset;
	void *ptr_e_stroff = (char *)ptr + s_head_str->sh_offset;
	if (sort_print_data_64(ptr_e_stroff, symbol_nbr, ptr_e_symoff, s_head_first) == 1)
		return (1);
	else
		return (0);
}

int main(int argc, char *argv[])
{
	void *ptr = parsing_mmaping(argc, argv);
	if (ptr == NULL)
		return (1);
	Elf64_Ehdr *e_head = (Elf64_Ehdr *)ptr;
	e_type = e_head->e_type;
	if (e_head->e_ident[EI_CLASS] == ELFCLASS32)
		return (find_sym_str_tab_32(ptr));
	else
		return (find_sym_str_tab_64(ptr));
}
