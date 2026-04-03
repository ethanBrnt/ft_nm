/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eburnet <eburnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/12 11:51:06 by eburnet           #+#    #+#             */
/*   Updated: 2026/04/03 12:03:32 by eburnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

int var;

char find_sym_type(Elf64_Sym sym_tab_elem, Elf64_Shdr *s_head_first)
{
	(void)var;
	int sh_type= -1;
	int sh_flags = -1;
	int st_type = ELF64_ST_TYPE(sym_tab_elem.st_info);
	int st_bind = ELF64_ST_BIND(sym_tab_elem.st_info);
	ft_printf("DEBUG: st_type:%d st_bind:%d\n", st_type, st_bind);
	if (sym_tab_elem.st_shndx == SHN_ABS && st_type != STT_FILE)
		return ('A');
	else if (sym_tab_elem.st_shndx == SHN_COMMON)
		return ('C');
	else if (sym_tab_elem.st_shndx >= SHN_LORESERVE)
		return ('e');
	else if (sym_tab_elem.st_shndx == SHN_UNDEF)
	{
		if ((st_type == STT_NOTYPE || st_type == STT_FUNC) && (st_bind == STB_LOCAL || st_bind == STB_WEAK))
			return ('w');
		else if ((st_type == STT_NOTYPE || st_type == STT_FUNC) && st_bind == STB_GLOBAL)
			return ('U');
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

	/* 
		< DEBUG: st_type:1 st_bind:1
		< DEBUG:  sh_type:1 sh_flags:2
		< 0000000000003000 r _IO_stdin_used
		---
		> 0000000000003000 R _IO_stdin_used
		--------------------------------------
		DEBUG: st_type:1 st_bind:0
		DEBUG:  sh_type:1 sh_flags:2
		0000000000003578 r __FRAME_END__
		< DEBUG: st_type:1 st_bind:0
		< DEBUG:  sh_type:7 sh_flags:2
		< 000000000000038c R __abi_tag
		---
		> 000000000000038c r __abi_tag

		-------------------------------------
		>                  U write@GLIBC_2.2.5


		main.o
		187a94
		> 0000000000000000 n wm4.wordsize.h.4.baf119258a1e53d8dba67ceac44ab6bc
	*/
}

// int	do32(void *ptr, void *ptr_e_shoff, int section_nbr, int shead_size)
// {
// 	// redoo
// 	return (0);
// }

char *padding_management(int st_value, char type)
{
	char *padding = malloc(sizeof(char) * 17);
	if (!padding)
		return (NULL);
	memset(padding, '0', 16);
	padding[16] = '\0';
	int len = 0;
	unsigned long addr = st_value;
	if (st_value == 0)
		len = 1;
	while (addr > 0)
	{
		addr = addr / 16;
		len++;
	}
	memset(padding + (16 - len), '\0', 1);
	if (type == 'U' || type == 'w')
		memset(padding, ' ', 16);
	return (padding);
}

int sort_print_data(void *ptr_e_stroff, int symbol_nbr, void *ptr_e_symoff, Elf64_Shdr *s_head_first )
{
	Elf64_Sym *sym_table = (Elf64_Sym *)ptr_e_symoff;
	char *str_table = (char *)ptr_e_stroff;
	int *strTabIndex = malloc(sizeof(int) * symbol_nbr);
	if (!strTabIndex)
		return (1);
	ft_memset(strTabIndex, 0, sizeof(int) * symbol_nbr);
	int strTabLen = 0;
	for (int i = 0; i < symbol_nbr; i++)
	{
		if (sym_table[i].st_name != 0)
			strTabIndex[strTabLen++] = sym_table[i].st_name;
	}
	quick_sort(strTabIndex, 0, strTabLen, str_table);
	for (int i = 0; i < strTabLen; i++)
	{
		int sym_index = -1;
		for (int j = 0; j < symbol_nbr; j++)
		{
			if ((int)sym_table[j].st_name == strTabIndex[i])
				sym_index = j;
		}
		if (sym_index == -1)
			return (ft_putstr_fd("ft_nm: sym_index not found\n", 2), 1);
		char *name = str_table + strTabIndex[i];
		if (!name || ft_strlen(name) == 0)
			continue;
		char type = find_sym_type(sym_table[sym_index], s_head_first);
		if (type == 'e')
			continue;
		char *padding = padding_management(sym_table[sym_index].st_value, type);
		if (!padding)
			return (free(strTabIndex), 1);
		if (type == 'U' || type == 'w')
			ft_printf("%s %c %s\n", padding, type, name);
		else
			ft_printf("%s%x %c %s\n", padding, sym_table[sym_index].st_value, type, name);
		free(padding);
	}
	free(strTabIndex);
	return (0);
}

int find_sym_str_tab_64(void *ptr, void *ptr_e_shoff, int section_nbr)
{
	Elf64_Shdr *s_head_first = (Elf64_Shdr *)ptr_e_shoff;
	Elf64_Shdr *s_head_sym;
	Elf64_Shdr *s_head_str;
	Elf64_Ehdr *e_head = (Elf64_Ehdr *)ptr;
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
	if (sort_print_data(ptr_e_stroff, symbol_nbr, ptr_e_symoff, s_head_first) == 1)
		return (1);
	else
		return (0);
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
	// verifier que le fichier est nul terminer
	// Faire la verif de la taille du fichier par rapport a celle annoncer par fstat
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

int main(int argc, char *argv[])
{
	void *ptr = parsing_mmaping(argc, argv);
	if (ptr == NULL)
		return (1);
	// Est-ce un pb de caster en 64 directement ? Tester avec fichier 32 subject
	Elf64_Ehdr *e_head = (Elf64_Ehdr *)ptr;
	if (e_head->e_ident[EI_MAG0] != ELFMAG0 || e_head->e_ident[EI_MAG1] != ELFMAG1 || e_head->e_ident[EI_MAG2] != ELFMAG2 || e_head->e_ident[EI_MAG3] != ELFMAG3)
		return (ft_putstr_fd("ft_nm: file format not recognized\n", 2), 1);
	if (e_head->e_shstrndx != e_head->e_shnum - 1)
		return (ft_putstr_fd("ft_nm: file format not recognized\n", 2), 1);

	bool arch32 = false;
	if (e_head->e_ident[EI_CLASS] == ELFCLASS32)
		arch32 = true;
	void *ptr_e_shoff = (char *)ptr + e_head->e_shoff;
	int section_nbr = e_head->e_shnum;
	(void)arch32;
	// if (arch32)
	// 	return (do32(ptr, ptr_e_shoff, section_nbr));
	// else
		return (find_sym_str_tab_64(ptr, ptr_e_shoff, section_nbr));
}
