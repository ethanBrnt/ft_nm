/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eburnet <eburnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/12 11:51:06 by eburnet           #+#    #+#             */
/*   Updated: 2026/04/01 16:07:42 by eburnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

/* char find_sym_type(Elf64_Shdr *s_head_first, Elf64_Sym *sym_table)
{
	int Stype;
	int Sflags;
	if (sym_table[i].st_shndx >= SHN_LORESERVE)
	{
		Stype = -1;
		Sflags = -1;
	}
	else if (sym_table[i].st_shndx == SHN_UNDEF)
	{
		Stype = 0;
		Sflags = 0;
	}
	else
	{
		Sflags = s_head_first[sym_table[i].st_shndx].sh_flags;
		Stype = s_head_first[sym_table[i].st_shndx].sh_type;
	}
	ft_printf("%X %d %d %s\n", sym_table[i].st_value, ELF64_ST_TYPE(sym_table[i].st_info), ELF64_ST_BIND(sym_table[i].st_info), name);
		// if ELF64_ST_TYPE() >= 0 && ELF64_ST_TYPE() <= 2 || name != "" || name != null
		// print
		// le TYPE et le BIND modifie tout les deux la valeur du symbole maj min
		// mais auusi la lettre en fonction de st_shndx cela donne acces a la sectiion
		// d'ou provient le symbole et on regarder sh_flags et sh_type pour definir la
		// lettre. SI st_shndx == SHN_UNDEF alors la lettre seras u ou w

	// Table de corsepondance

	// if (ELF64_ST_BIND() == STB_LOCAL || ELF64_ST_BIND() == STB_WEAK)
		// min
	// else if (ELF64_ST_BIND() == STB_GLOBAL)
		// maj
} */

// int	do32(void *ptr, void *ptr_e_shoff, int section_nbr, int shead_size)
// {
// 	// redoo
// 	return (0);
// }

char *padding_management(int st_value, char type)
{
	char *padding = malloc(sizeof(char) * 16);
	if (!padding)
		return (NULL);
	memset(padding, '0', 16);
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
	if (type == 'U')
		memset(padding, ' ', 16);
	return (padding);
}

int do64(void *ptr, void *ptr_e_shoff, int section_nbr)
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
	Elf64_Sym *sym_table = (Elf64_Sym *)ptr_e_symoff;
	void *ptr_e_stroff = (char *)ptr + s_head_str->sh_offset;
	char *str_table = (char *)ptr_e_stroff;
	int *tabStrIndex = malloc(sizeof(int) * symbol_nbr);

	int strTabLen = 0;
	for (int i = 0; i < symbol_nbr; i++)
	{
		if (sym_table[i].st_name != 0)
		{
			tabStrIndex[strTabLen] = sym_table[i].st_name;
			ft_printf("%d, ", tabStrIndex[strTabLen]); //
			strTabLen++;
		}
	}
	ft_printf("\n");
	quick_sort(tabStrIndex, 0, strTabLen, str_table);
	for (int i = 0; i < strTabLen; i++)
	{
		ft_printf("%d: %s\n ", tabStrIndex[i], str_table + tabStrIndex[i]); //
	}
	// trie en ordre alphabetic de str_table
	// int tab[];
	// stock index parcour sym_table
	// creer tableau, boucle for stocker les indexes 0 - n
	// parcourir le tableau depuis 0
	// int res = ft_strcmp(name1, name2)
	//  Le tri rapide

	for (int i = 0; i < symbol_nbr; i++)
	{
		char *name = str_table + sym_table[i].st_name;
		if (!name || ft_strlen(name) == 0)
			continue;
		// char type = find_sym_type(s_head_first, sym_table);
		char type = 'z';
		char *padding = padding_management(sym_table[i].st_value, type);
		if (!padding)
			return (1);
		ft_printf("%s%X %c %s\n", padding, sym_table[i].st_value, type, name);
		free(padding);
	}
	return (0);
}

int main(int argc, char *argv[])
{
	int file_length = 0;
	char *filename = "a.out";
	bool arch32 = false;
	if (argc > 2)
		return (perror("ft_nm: Too much args"), 1);
	if (argc == 2)
	{
		if (argv[1] == NULL || ft_strncmp(argv[1], "", 1) == 0)
			return (1);
		else
			filename = argv[1];
	}
	int fd_file = open(filename, O_RDONLY);
	if (fd_file < 0)
	{
		ft_putstr_fd("ft_nm: '", 2);
		ft_putstr_fd(filename, 2);
		perror("'");
		return (1);
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
		return (1);
	}
	file_length = fstat_res.st_size;
	void *ptr = mmap(NULL, file_length, PROT_READ, MAP_PRIVATE, fd_file, 0);
	if (ptr == MAP_FAILED)
		return (ft_putstr_fd("ft_nm: mmap failed\n", 2), 1);
	// Est-ce un pb de caster en 64 directement ? Tester avec fichier 32 subject
	Elf64_Ehdr *e_head = (Elf64_Ehdr *)ptr;
	if (e_head->e_ident[EI_MAG0] != ELFMAG0 || e_head->e_ident[EI_MAG1] != ELFMAG1 || e_head->e_ident[EI_MAG2] != ELFMAG2 || e_head->e_ident[EI_MAG3] != ELFMAG3)
		return (ft_putstr_fd("ft_nm: file format not recognized\n", 2), 1);
	if (e_head->e_shstrndx != e_head->e_shnum - 1)
		return (ft_putstr_fd("ft_nm: file format not recognized\n", 2), 1);
	if (e_head->e_ident[EI_CLASS] == ELFCLASS32)
		arch32 = true;
	void *ptr_e_shoff = (char *)ptr + e_head->e_shoff;
	int section_nbr = e_head->e_shnum;
	(void)arch32;
	// if (arch32)
	// 	return (do32(ptr, ptr_e_shoff, section_nbr));
	// else
	return (do64(ptr, ptr_e_shoff, section_nbr));
}
