/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eburnet <eburnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/12 11:51:06 by eburnet           #+#    #+#             */
/*   Updated: 2026/03/18 16:13:53 by eburnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

int main(int argc, char *argv[])
{
	int file_length = 0;
	int fd_file;
	char *filename = "a.out";
	struct stat fstat_res;
	if (argc > 2)
	{
		perror("ft_nm: Too much args");
		return (1);
	}
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
	void *ptr = mmap(NULL, file_length, PROT_READ, MAP_PRIVATE, fd_file, getpagesize());
	Elf64_Ehdr *e_head = (Elf64_Ehdr *)ptr;
	if (!(e_head->e_ident[EI_MAG0] == ELFMAG0 || e_head->e_ident[EI_MAG1] == ELFMAG1 || e_head->e_ident[EI_MAG2] == ELFMAG2 || e_head->e_ident[EI_MAG3] == ELFMAG3))
		return (ft_putstr_fd("ft_nm: file format not recognized\n", 2), 1);
	printf("ACTUALL END\n");
	// QUESTION:
		// de quoi est composer le header ?
			/* composer en premier de e_ident (un tableau d'octet) qui permet de definir le 
				type de fichier. Avec EI_MAG0-3 qui doivent avoir pour valeur 
				nom dans le include    (ELFMAG0  ELFMAG1  ELFMAG2  ELFMAG3)
				valeur dans le include ( 0x7f       E        L        F   )
				et EI_CLASS qui definit l'archi system, valeur a ELFCLASS32 | ELFCLASS64 */
			// il faut donc commencer par lire e_ident qui est toujour le meme, 
			// verifier que c'est un ELF, et enregistrer sa class (32 ou 64)
			/* Ensuite e_type ET_REL Un fichier repositionnable(.o), 
				ET_EXEC Un fichier exécutable, ET_DYN Un objet partagé (.so). */
		// comment parcourir les element du header ou autre dans mmap ?
			// en castant le ptr a l'adresse choisi
			// Elf64_Ehdr *header = (Elf64_Ehdr *)ptr;
			// ou en ptr1 = (char *)ptr + e_shoff pour se deplacer a l'adresse de e_shoff
			// puis cast du ptr1 dans la struc ou var approprier
		
		// nm affiche l'adresse memoire, le type, le nom du symbole ()
			// un symbole est tout cd qui a un nom et une adresse memoire 
				// (var global, fonction, var static)
		// .strtab - symbol table, il faut donc lire dans la symbole table des fichier ELF
		// comment acceder a la symbol table ?
		/* Elf64_Ehdr -> e_shoff adresse pour acceder au tableau d'entete (header) DE SECTION,
						 e_shnum nombre de section dans le tableau de header
						 e_shentsize taille du header de la section qui est la meme pour tout les header
		e_shentsize x e_shnum = taille en octets de la table des entêtes de sections
		Il faut parcourir le tableau de Elf[64-32]_Shdr et trouve sh_type == SHT_SYMTAB 
		sh_type est toujour le 2nd element du tableau
		Si le sh_type == SHT_SYMTAB alors la section contient .symtab */
		/* Elf[32-64]_Sym comporte les memes champs mais pas dans le meme ordre
			st_name (nom du symbole), st_value (addr ?), st_info (type du symbole) */
		// pour acceder au Elfn_Sym aller a (char)ptr + sh_offset de Elfn_Shdr
		// sh_entsize donne le nombre de symbole present dans le tableau
}