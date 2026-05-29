/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eburnet <eburnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 10:01:20 by eburnet           #+#    #+#             */
/*   Updated: 2026/05/29 20:46:36 by eburnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

char find_sym_type_64(Elf64_Sym sym_tab_elem, Elf64_Shdr *s_head_first)
{
	int sh_type= -1;
	int sh_flags = -1;
	int st_type = ELF64_ST_TYPE(sym_tab_elem.st_info);
	int st_bind = ELF64_ST_BIND(sym_tab_elem.st_info);
	ft_printf("DEBUG: st_type:%d st_bind:%d\n", st_type, st_bind);
	if (sym_tab_elem.st_shndx == SHN_ABS && st_type != STT_FILE && e_type != ET_DYN)
		return ('A');
	else if (sym_tab_elem.st_shndx == SHN_ABS && st_type != STT_FILE && e_type == ET_DYN)
		return ('a');
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
	ft_printf("DEBUG:  sh_type:%d sh_flags:%d\n", sh_type, sh_flags);
	if (st_type == STT_FUNC && sh_type == SHT_PROGBITS && sh_flags & SHF_EXECINSTR && sh_flags & SHF_ALLOC)
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

char find_sym_type_32(Elf32_Sym sym_tab_elem, Elf32_Shdr *s_head_first)
{
	int sh_type= -1;
	int sh_flags = -1;
	int st_type = ELF64_ST_TYPE(sym_tab_elem.st_info);
	int st_bind = ELF64_ST_BIND(sym_tab_elem.st_info);
	// ft_printf("DEBUG: st_type:%d st_bind:%d\n", st_type, st_bind);
	if (sym_tab_elem.st_shndx == SHN_ABS && st_type != STT_FILE && e_type != ET_DYN)
		return ('A');
	else if (sym_tab_elem.st_shndx == SHN_ABS && st_type != STT_FILE && e_type == ET_DYN)
		return ('a');
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
	if (st_type == STT_FUNC && sh_type == SHT_PROGBITS && sh_flags & SHF_EXECINSTR && sh_flags & SHF_ALLOC)
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

char *padding_management(int st_value, char type, bool is_32)
{
    int bits = 16;
	char *padding = malloc(sizeof(char) * 17);
	if (!padding)
		return (NULL);
	if (is_32)
		bits = 8;
	memset(padding, '0', bits);
	padding[bits] = '\0';
	int len = 0;
	unsigned long addr = st_value;
	if (st_value == 0)
		len = 1;
	while (addr > 0)
	{
		addr = addr / 16;
		len++;
	}
	memset(padding + (bits - len), '\0', 1);
	if (type == 'U' || type == 'w')
		memset(padding, ' ', bits);
	return (padding);
}

void *parsing_mmaping(int argc, char *argv[])
{
	int file_length = 0;
	char *filename = "a.out";
	if (argc > 2)
		return (perror("ft_nm: Too much args"), NULL);
	if (argc == 2)
	{
		if (argv[1] == NULL || ft_strncmp(argv[1], "", 1) == 0)
			return (NULL);
		else
			filename = argv[1];
	}
	int fd_file = open(filename, O_RDONLY);
	if (fd_file < 0)
	{
		ft_putstr_fd("ft_nm: '", 2);
		ft_putstr_fd(filename, 2);
		perror("'");
		return (NULL);
	}
	struct stat fstat_res;
	fstat(fd_file, &fstat_res);
	if (!S_ISREG(fstat_res.st_mode))
	{
		ft_putstr_fd("ft_nm: Warning: '", 2);
		ft_putstr_fd(filename, 2);
		ft_putstr_fd("' is a directory\n", 2);
		return (NULL);
	}
	file_length = fstat_res.st_size;
	void *ptr = mmap(NULL, file_length, PROT_READ, MAP_PRIVATE, fd_file, 0);
	if (ptr == MAP_FAILED)
		return (ft_putstr_fd("ft_nm: mmap failed\n", 2), NULL);
	return (ptr);
}