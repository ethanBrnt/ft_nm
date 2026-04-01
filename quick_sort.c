/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quick_sort.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eburnet <eburnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 11:49:33 by eburnet           #+#    #+#             */
/*   Updated: 2026/04/01 17:01:23 by eburnet          ###   ########.fr       */
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
		while (i <= end && ft_strcmp(str_table + pivot, str_table + tab[i]) > 0)
		{
			printf("i: %d\n", i);
			i++;
		}
		while (j >= start && ft_strcmp(str_table + pivot, str_table + tab[j]) < 0)
		{
			printf("j: %d\n", j);
			j--;
		}
		if (i <= j)
		{
			printf("swap i: %d, j: %d\n", tab[i], tab[j]);
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
