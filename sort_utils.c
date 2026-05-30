/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eburnet <eburnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 11:49:33 by eburnet           #+#    #+#             */
/*   Updated: 2026/05/30 12:55:54 by eburnet          ###   ########.fr       */
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
	// printf("s1: %s, s2: %s \n", s1, s2);
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
