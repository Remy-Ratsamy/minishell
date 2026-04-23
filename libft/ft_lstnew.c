/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gechavia <gechavia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 18:58:40 by gechavia          #+#    #+#             */
/*   Updated: 2025/04/30 22:00:24 by gechavia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*caca;

	caca = malloc(sizeof(t_list));
	if (!caca)
		return (NULL);
	caca->content = content;
	caca->next = NULL;
	return (caca);
}
