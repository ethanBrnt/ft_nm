/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort32.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eburnet <eburnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 12:56:36 by eburnet           #+#    #+#             */
/*   Updated: 2026/05/30 13:07:56 by eburnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

void quick_sort_32(Elf32_Sym *sym_table, int *sym_tab_index, int start, int end, char *str_table)
{
	int i = start;
	int j = end;
	int pivot = sym_tab_index[(start + end) / 2];
	while (i <= j)
	{
		while (i <= end && ft_strcmp_skip_special_char(str_table + sym_table[pivot].st_name, str_table + sym_table[sym_tab_index[i]].st_name) > 0)
			i++;
		while (j >= start && ft_strcmp_skip_special_char(str_table + sym_table[pivot].st_name, str_table + sym_table[sym_tab_index[j]].st_name) < 0)
			j--;
		if (i <= j)
		{
			swap(&sym_tab_index[i], &sym_tab_index[j]);
			i++;
			j--;
		}
	}
	if (start < j)
		quick_sort_32(sym_table, sym_tab_index, start, j, str_table);
	if (i < end)
		quick_sort_32(sym_table, sym_tab_index, i, end, str_table);
}

void stable_fix_32(Elf32_Sym *sym_table, int *sym_tab_index, int printable_sym_nbr, char *str_table)
{
    int i = 0;
    while (i < printable_sym_nbr)
    {
        int j = i + 1;
        while (j < printable_sym_nbr && ft_strcmp_skip_special_char(str_table + sym_table[sym_tab_index[i]].st_name, str_table + sym_table[sym_tab_index[j]].st_name) == 0)
            j++;
        for (int k = i + 1; k < j; k++)
        {
            int key = sym_tab_index[k];
            int l = k - 1;
            while (l >= i && sym_tab_index[l] > key)
            {
                sym_tab_index[l + 1] = sym_tab_index[l];
                l--;
            }
            sym_tab_index[l + 1] = key;
        }
        i = j;
    }
}