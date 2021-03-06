/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   recursive_parser.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdaveux <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/22 13:23:36 by cdaveux           #+#    #+#             */
/*   Updated: 2022/06/02 12:28:26 by cdaveux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_here(t_token **tmp, t_token **inf, t_token **out, t_token **cmd)
{
	if (!(*tmp)->next)
		return (error_msg(1, NULL));
	while ((*tmp)->token == SPACE && (*tmp)->next)
		ft_free(tmp);
	if ((*tmp)->next)
	{
		ft_free(tmp);
		if ((*tmp)->token == SPACE)
			ft_free(tmp);
		if ((*tmp)->token == 9 || (*tmp)->token == 1 || (*tmp)->token == 2
			|| (*tmp)->token == 3 || (*tmp)->token == 6 || (*tmp)->token == 11)
		{
			join_data(tmp);
			(*tmp)->token = 8;
			push(tmp, inf);
		}
	}
	else
		return (error_msg(1, NULL));
	if (!(*tmp))
		return (0);
	if ((*tmp)->token == 0)
		return (-1);
	return (check_word(tmp, inf, out, cmd));
}

int	check_word(t_token **tmp, t_token **inf, t_token **out, t_token **cmd)
{
	if (!(*tmp))
		return (0);
	while ((*tmp) && (*tmp)->token == SPACE)
		ft_free(tmp);
	if (!(*tmp))
		return (0);
	if ((*tmp)->token == PIPE && (*tmp)->next != NULL)
		return (-1);
	if ((*tmp)->token == 9 || (*tmp)->token == 1 || (*tmp)->token == 2
		|| (*tmp)->token == 3 || (*tmp)->token == 6 || (*tmp)->token == 11)
	{
		join_data(tmp);
		push(tmp, cmd);
		return (check_word(tmp, inf, out, cmd));
	}
	if ((*tmp)->token == 5 || (*tmp)->token == 7
		|| (*tmp)->token == 4 || (*tmp)->token == 8)
		return (check_token(tmp, inf, out, cmd));
	return (error_msg(0, (*tmp)->data));
}

int	check_inf(t_token **tmp, t_token **inf, t_token **out, t_token **cmd)
{
	if (!(*tmp)->next)
		return (error_msg(1, NULL));
	if ((*tmp)->next->token == SPACE)
		ft_free(tmp);
	if (!(*tmp)->next)
		return (error_msg(1, NULL));
	if ((*tmp)->next->token != 9)
		return (error_msg(0, (*tmp)->next->data));
	else
	{
		while ((*tmp)->token == SPACE && (*tmp)->next)
			ft_free(tmp);
		if ((*tmp)->token == REDIR_IN)
			ft_free(tmp);
		(*tmp)->token = 4;
		push(&(*tmp), inf);
		if (!(*tmp))
			return (0);
		if ((*tmp)->token == 0)
			return (-1);
		return (check_word(&(*tmp), inf, out, cmd));
	}
}

int	check_out(t_token **tmp, t_token **inf, t_token **out, t_token **cmd)
{
	if (!(*tmp)->next)
		return (error_msg(1, NULL));
	while ((*tmp)->token == SPACE && (*tmp)->next)
		ft_free(tmp);
	if ((*tmp)->next->token == 9 || ((*tmp)->next->token == SPACE
			&& (*tmp)->next->next->token == 9))
	{
		ft_free(tmp);
		if ((*tmp)->token == SPACE)
			ft_free(tmp);
		if ((*tmp)->token == 9)
			(*tmp)->token = 5;
		push(&(*tmp), out);
	}
	else
		return (error_msg(0, (*tmp)->next->data));
	if (!(*tmp))
		return (0);
	if ((*tmp)->token == 0)
		return (-1);
	return (check_word(&(*tmp), inf, out, cmd));
}

int	check_out_b(t_token **tmp, t_token **inf, t_token **out, t_token **cmd)
{
	if (!(*tmp)->next)
		return (error_msg(1, NULL));
	while ((*tmp)->token == SPACE && (*tmp)->next)
		ft_free(tmp);
	if ((*tmp)->next->token == 9 || ((*tmp)->next->token == SPACE
			&& (*tmp)->next->next->token == 9))
	{
		ft_free(tmp);
		if ((*tmp)->token == SPACE)
			ft_free(tmp);
		if ((*tmp)->token == 9)
			(*tmp)->token = 7;
		push(&(*tmp), out);
	}
	else
		return (error_msg(0, (*tmp)->next->data));
	if (!(*tmp))
		return (0);
	if ((*tmp)->token == 0)
		return (-1);
	return (check_word(&(*tmp), inf, out, cmd));
}
