/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eburnet <eburnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/12 11:51:06 by eburnet           #+#    #+#             */
/*   Updated: 2026/03/24 15:09:03 by eburnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

// int	do32(void *ptr, void *ptr_e_shoff, int section_nbr, int shead_size)
// {
// 	// redoo
// 	return (0);
// }

int	do64(void *ptr, void *ptr_e_shoff, int section_nbr)
{
	Elf64_Shdr *s_head_first = (Elf64_Shdr *)ptr_e_shoff;
	Elf64_Shdr *s_head_sym;
	Elf64_Shdr *s_head_str;
	Elf64_Ehdr *e_head = (Elf64_Ehdr *)ptr;
	for (int i = 0; i < section_nbr; i++)
	{
		Elf64_Shdr *s_head = (Elf64_Shdr *)ptr_e_shoff;
		// ft_printf("s_head->sh_type: %d\n", s_head->sh_type);
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
	for (int i = 0; i < symbol_nbr; i++)
	{
		//.strtab pour name, infos ELF64_ST_TYPE et ELF64_ST_BIND
		// trie en ordre alphabetic quel algo ?
		// le padding de l'adresse avec printf
		// if ELF64_ST_TYPE() >= 0 && ELF64_ST_TYPE() <= 2 || name != "" || name != null
			// print
			// le TYPE et le BIND modifie tout les deux la valeur du symbole maj min et +
			// st_shndx est a pren dre en compte en fonction de la section
			// a laquelle est lier le symbole
		
		// if (ELF64_ST_BIND() == STB_LOCAL || ELF64_ST_BIND() == STB_WEAK)
			// min
		// else if (ELF64_ST_BIND() == STB_GLOBAL)
			// maj
		char *name = str_table + sym_table[i].st_name;
		ft_printf("%X %d %d %s\n", sym_table[i].st_value, ELF64_ST_TYPE(sym_table[i].st_info), ELF64_ST_BIND(sym_table[i].st_info), name);
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
