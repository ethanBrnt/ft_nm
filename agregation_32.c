/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   agregation_32.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eburnet <eburnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 11:58:12 by eburnet           #+#    #+#             */
/*   Updated: 2026/05/30 13:35:16 by eburnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

char find_sym_type_32(Elf32_Sym sym_tab_elem, Elf32_Shdr *s_head_first)
{
	int sh_type= -1;
	int sh_flags = -1;
	int st_type = ELF64_ST_TYPE(sym_tab_elem.st_info);
	int st_bind = ELF64_ST_BIND(sym_tab_elem.st_info);
	// ft_printf("DEBUG: st_type:%d st_bind:%d\n", st_type, st_bind);
	if (sym_tab_elem.st_shndx == SHN_ABS && st_type != STT_FILE)
	{
		if (e_type == ET_DYN)
			return ('a');
		else
			return ('A');
	}
	else if (sym_tab_elem.st_shndx == SHN_COMMON)
		return ('C');
	else if (sym_tab_elem.st_shndx >= SHN_LORESERVE)
		return ('e');
	else if (sym_tab_elem.st_shndx == SHN_UNDEF)
	{
		if ((st_type == STT_NOTYPE || st_type == STT_FUNC) && (st_bind == STB_LOCAL || st_bind == STB_WEAK))
			return ('w');
		else if ((st_type == STT_NOTYPE || st_type == STT_TLS || st_type == STT_FUNC || st_type == STT_OBJECT) && st_bind == STB_GLOBAL)
			return ('U');
		else if (st_type == STT_OBJECT && (st_bind == STB_LOCAL || st_bind == STB_WEAK))
			return ('v');
	}
	sh_type = s_head_first[sym_tab_elem.st_shndx].sh_type;
	sh_flags = s_head_first[sym_tab_elem.st_shndx].sh_flags;
	// ft_printf("DEBUG:  sh_type:%d sh_flags:%d\n", sh_type, sh_flags);
	if ((st_type == STT_FUNC || st_type == STT_NOTYPE) && sh_type == SHT_PROGBITS && sh_flags & SHF_EXECINSTR && sh_flags & SHF_ALLOC)
	{
		if (st_bind == STB_GLOBAL)
			return ('T');
		else if (st_bind == STB_LOCAL || st_bind == STB_WEAK)
			return ('t');
	}
	else if (sh_flags & SHF_ALLOC && sh_flags & SHF_WRITE && !(sh_flags & SHF_EXECINSTR))
	{
		if (sh_type == SHT_NOBITS)
		{
			if (st_bind == STB_GLOBAL)
				return ('B');
			else if (st_bind == STB_LOCAL || st_bind == STB_WEAK)
				return ('b');
		}
		else if (sh_type != SHT_NOBITS)
		{
			if (st_bind == STB_GLOBAL)
				return ('D');
			else if (st_bind == STB_LOCAL)
				return ('d');
			else if (st_bind == STB_WEAK)
				return ('W');
		}
	}
	else if (sh_flags & SHF_ALLOC && !(sh_flags & SHF_WRITE) && !(sh_flags & SHF_EXECINSTR))
	{
		if (st_bind == STB_LOCAL || st_bind == STB_WEAK)
			return ('r');
		else if (st_bind == STB_GLOBAL)
			return ('R');
	}
	else if (st_type == STT_NOTYPE && st_bind == STB_LOCAL)
		return ('n');
	return ('?');
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

int sort_print_data_32(void *ptr_e_stroff, int symbol_nbr, void *ptr_e_symoff, Elf32_Shdr *s_head_first)
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
	quick_sort_32(sym_table, sym_tab_index, 0, printable_sym_nbr - 1, str_table);
	stable_fix_32(sym_table, sym_tab_index, printable_sym_nbr - 1, str_table);
	for (int i = 0; i < printable_sym_nbr; i++)
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