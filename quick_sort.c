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
		while (*s1 && !ft_isalnum(*s1))
			s1++;
		while (*s2 && !ft_isalnum(*s2))
			s2++;
		char c1 = ft_tolower(*s1);
		char c2 = ft_tolower(*s2);
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

void quick_sort(int *tab, int start, int end, char *str_table )
{
	int i = start;
	int j = end;
	int pivot = tab[(start + end) / 2];
	while (i <= j)
	{
		while (i <= end && tab[i] && ft_strcmp_skip_special_char(str_table + pivot, str_table + tab[i]) > 0)
		{
			i++;
		}	
		while (j >= start && tab[j] && ft_strcmp_skip_special_char(str_table + pivot, str_table + tab[j]) < 0)
		{
			j--;
		}	
		if (i <= j)
		{
			swap(&tab[i], &tab[j]);
			i++;
			j--;
		}
	}
	if (start < j)
		quick_sort(tab, start, j, str_table);
	if (i < end)
		quick_sort(tab, i, end, str_table);
}
