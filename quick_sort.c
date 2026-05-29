/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quick_sort.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eburnet <eburnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 11:49:33 by eburnet           #+#    #+#             */
/*   Updated: 2026/04/02 12:32:11 by eburnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return (*(unsigned char *)s1 - *(unsigned char *)s2);
}

int ft_strcmp_skip_special_char(char *s1, char *s2)
{
	char *origin_s1 = s1;
	char *origin_s2 = s2;

	while (*s1 || *s2)
	{
		char c1 = *s1;
		char c2 = *s2;
		while (c1 && !ft_isalnum(c1) && c1 != '$')
		{
			s1++;
			c1 = *s1;
		}
		while (c2 && !ft_isalnum(c2) && c2 != '$')
		{
			s2++;
			c2 = *s2;
		}
		c1 = ft_tolower(*s1);
		c2 = ft_tolower(*s2);
		if (c1 != c2)
			return (c1 - c2);
		s1++;
		s2++;
	}
	return (ft_strcmp(origin_s1, origin_s2));
}

void	swap(int *a, int *b)
{
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

void quick_sort_64(Elf64_Sym *sym_table, int *sym_tab_index, int start, int end, char *str_table)
{
	int i = start;
	int j = end;
	int pivot = sym_tab_index[(start + end) / 2];
	while (i <= j)
	{
		// CHECK SANS && sym_tab_index[i] 
		// while (i <= end && sym_table[sym_tab_index[i]].st_value  && ft_strcmp_skip_special_char(str_table + pivot, str_table + sym_table[sym_tab_index[i]].st_name) > 0)
		while (i <= end && sym_table[sym_tab_index[i]].st_value  && ft_strcmp_skip_special_char(str_table + pivot, str_table + sym_tab_index[i]) > 0)
			i++;
		// CHECK SANS && sym_tab_index[j] 
		// while (j >= start && sym_table[sym_tab_index[j]].st_value && ft_strcmp_skip_special_char(str_table + pivot, str_table + sym_table[sym_tab_index[j]].st_name) < 0)
		while (j >= start && sym_table[sym_tab_index[j]].st_value && ft_strcmp_skip_special_char(str_table + pivot, str_table + sym_tab_index[j]) < 0)
			j--;
		if (i <= j)
		{
			swap(&sym_tab_index[i], &sym_tab_index[j]);
			i++;
			j--;
		}
	}
	if (start < j)
		quick_sort_64(sym_table, sym_tab_index, start, j, str_table);
	if (i < end)
		quick_sort_64(sym_table, sym_tab_index, i, end, str_table);
}

void quick_sort_32(Elf32_Sym *sym_table, int *sym_tab_index, int start, int end, char *str_table)
{
	int i = start;
	int j = end;
	int pivot = sym_tab_index[(start + end) / 2];
	while (i <= j)
	{
		// CHECK SANS && sym_tab_index[i] 
		while (i <= end && sym_tab_index[i] && ft_strcmp_skip_special_char(str_table + pivot, str_table + sym_table[sym_tab_index[i]].st_name) > 0)
			i++;
		// CHECK SANS && sym_tab_index[j] 
		while (j >= start && sym_tab_index[j] && ft_strcmp_skip_special_char(str_table + pivot, str_table + sym_table[sym_tab_index[j]].st_name) < 0)
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


/* 

// sort adress et find name from address 
➜  tests git:(master) ✗ ./test.sh ../test.so 

==== SAME SYM SAME ADDR ??? ====
1,16c1,16
< 00000000000012ac r $d
< 00000000000012ac r $d
< 00000000000012ac r $d
< 00000000000012ac r $d
< 00000000000012ac r $d
< 00000000000012ac r $d
< 00000000000012ac r $d
< 00000000000012ac r $d
< 0000000000000840 n $x
< 0000000000000840 n $x
< 0000000000000840 n $x
< 0000000000000840 n $x
< 0000000000000840 n $x
< 0000000000000840 n $x
< 0000000000000840 n $x
< 0000000000000840 n $x
---
> 0000000000020080 d $d
> 000000000001fde0 d $d
> 000000000001fdd8 d $d
> 00000000000011e4 r $d
> 0000000000020088 b $d
> 00000000000010d8 r $d
> 0000000000001248 r $d
> 00000000000012ac r $d
> 0000000000000960 t $x
> 0000000000000820 t $x
> 00000000000010c0 t $x
> 0000000000000830 t $x
> 00000000000010cc t $x
> 0000000000000980 t $x
> 0000000000000a48 t $x
> 0000000000000840 t $x


===== BASED ON EXTENSION (.so) =====
26c26
< 000000000001fde8 A _DYNAMIC
---
> 000000000001fde8 a _DYNAMIC
34c34
< 000000000001ffb8 A _GLOBAL_OFFSET_TABLE_
---
> 000000000001ffb8 a _GLOBAL_OFFSET_TABLE_
*/