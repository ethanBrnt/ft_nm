/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eburnet <eburnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/12 11:51:06 by eburnet           #+#    #+#             */
/*   Updated: 2026/05/30 12:55:52 by eburnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"
uint16_t e_type;

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
