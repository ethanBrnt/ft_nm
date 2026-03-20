/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eburnet <eburnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/12 11:51:06 by eburnet           #+#    #+#             */
/*   Updated: 2026/03/20 16:12:23 by eburnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

int	do32(void *ptr, void *ptr_e_shoff, int section_nbr, int section_size)
{
	Elf32_Shdr *s_head = (Elf32_Shdr *)ptr_e_shoff;
}

int	do64(void *ptr, void *ptr_e_shoff, int section_nbr, int section_size)
{
	Elf64_Shdr *s_head;
	for (size_t i = 0; i < section_nbr; i++)
	{
		s_head = (Elf64_Shdr *)ptr_e_shoff;
		if (s_head->sh_type == SHT_SYMTAB)
			break ;
		// a verifier
		ptr_e_shoff = ptr_e_shoff + section_size + sizeof(Elf64_Shdr);
	}
	if (s_head->sh_type != SHT_SYMTAB)
		return (perror("SymTab not found"), 1);
	int table_size = s_head->sh_entsize;
	void *ptr_e_symoff = (char)ptr + s_head->sh_offset;
	Elf64_Sym *sym_table;
	// verifier calcul
	while (((char)ptr_e_shoff - (char)ptr_e_symoff) <= section_size)
	{
		sym_table = (Elf64_Sym *)ptr_e_symoff;

		ptr_e_symoff = ptr_e_symoff + s_head->sh_offset;

		// comment parcourir la table de symbole ????
	}
	// .strtab - symbol table, il faut donc lire dans la symbole table des fichier ELF
	/* Elf[32-64]_Sym comporte les memes champs mais pas dans le meme ordre
		st_name (nom du symbole), st_value (addr ?), st_info (type du symbole) */

}

int main(int argc, char *argv[])
{
	int file_length = 0;
	int fd_file;
	char *filename = "a.out";
	struct stat fstat_res;
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
	fd_file = open(filename, O_RDONLY);
	if (fd_file < 0)
	{
		ft_putstr_fd("ft_nm: '", 2);
		ft_putstr_fd(filename, 2);
		perror("'");
		return (1);
	}
	// verifier que le fichier est nul terminer
		// ??

	fstat(fd_file, &fstat_res);
	file_length = fstat_res.st_size;
	void *ptr = mmap(NULL, file_length, PROT_READ, MAP_PRIVATE, fd_file, 0);
	// Faire la verif der la taille du fichier par rapport a celle annoncer par fstat
	// Est-ce un pb de caster en 64 directement ? Tester avec fichier 32 subject
	Elf64_Ehdr *e_head = (Elf64_Ehdr *)ptr;
	if (e_head->e_ident[EI_MAG0] != ELFMAG0 && e_head->e_ident[EI_MAG1] != ELFMAG1 && e_head->e_ident[EI_MAG2] != ELFMAG2 && e_head->e_ident[EI_MAG3] != ELFMAG3)
		return (ft_putstr_fd("ft_nm: file format not recognized\n", 2), 1);
	if (e_head->e_ident[EI_CLASS] == ELFCLASS32)
		arch32 = true;
	// pour les fichiers objs recup de type pour SHT_DYNSYM ??
	void *ptr_e_shoff = (char *)ptr + e_head->e_shoff;
	int section_nbr = e_head->e_shnum;
	int section_size = e_head->e_shentsize;
	if (arch32)
		do32(ptr, ptr_e_shoff, section_nbr, section_size);
	else
		do64(ptr, ptr_e_shoff, section_nbr, section_size);
}
