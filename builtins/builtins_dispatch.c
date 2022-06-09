/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_dispatch.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eruellan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/07 13:24:49 by eruellan          #+#    #+#             */
/*   Updated: 2022/06/09 15:50:00 by cdaveux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_cd_minus(t_data *data)
{
	char	**tmp;
	char	*prev;
	int		i;

	i = 0;
	prev = NULL;
	tmp = data->envp;
	while (tmp[i])
	{
		if (ft_strncmp("OLDPWD=", tmp[i], 7) == 0)
		{
			prev = ft_strchr(tmp[i], '=');
			break ;
		}
		i++;
	}
	if (prev[1])
		return (&prev[1]);
	return (NULL);
}

int	ft_cd(t_heads **line, t_data *data)
{
	int		ret;
	char	*new;

	ret = 0;
	if ((*line)->cmd->next && ft_strncmp((*line)->cmd->next->data, "-", 1) != 0)
		change_pwd(1, data, NULL);
	else
		new = getcwd(NULL, 0);
	if ((*line)->cmd->next)
	{
		if (ft_strncmp((*line)->cmd->next->data, "-", 1) != 0)
			ret = chdir((*line)->cmd->next->data);
		else
		{
			ret = chdir(ft_cd_minus(data));
			change_pwd(1, data, new);
		}
	}
	else
		ret = chdir(getenv("HOME"));
	clear_all_heads(line);
	if (ret == -1)
		return (1);
	change_pwd(2, data, NULL);
	return (0);
}

int	dispatch_builtins(t_heads **line, t_data *data)
{
	if (ft_strncmp((*line)->cmd->data, "echo", 4) == 0)
	{
		if (!(*line)->cmd->next)
		{
			printf("\n");
			return (0);
		}
		ft_free(&(*line)->cmd);
		return (ft_echo(&(*line)->cmd, line));
	}
	if (ft_strncmp((*line)->cmd->data, "pwd", 3) == 0 && !(*line)->cmd->next)
		return (ft_pwd(line));
	if (ft_strncmp((*line)->cmd->data, "env", 3) == 0 && !(*line)->cmd->next)
		return (ft_env(data, line));
	if (ft_strncmp((*line)->cmd->data, "export", 6) == 0 && !(*line)->cmd->next)
		return (ft_solo_export(data));
	return (1);
}

int	non_printable_builtins(t_heads **line, t_data *data)
{
	int	ret;

	ret = 1;
	if (ft_strncmp_len((*line)->cmd->data, "cd", 2) == 0)
		return (ft_cd(&(*line), data));
	if (ft_strncmp_len((*line)->cmd->data, "export", 6) == 0
		&& (*line)->cmd->next)
	{
		ft_free(&(*line)->cmd);
		ret = ft_export((*line)->cmd, data);
	}
	if (ft_strncmp_len((*line)->cmd->data, "unset", 5) == 0
		&& (*line)->cmd->next)
	{
		ft_free(&(*line)->cmd);
		ret = ft_unset((*line)->cmd, data);
	}
	clear_all_heads(line);
	return (ret);
}

int	is_non_print_builtins(t_token *token)
{
	t_token	*tmp;

	tmp = token;
	if (tmp)
	{
		if (ft_strncmp_len(token->data, "cd", 2) == 0)
			return (error_cd(token));
		if (ft_strncmp_len(token->data, "export", 6) == 0 && token->next)
			return (error_export(token->next));
		if (ft_strncmp_len(token->data, "unset", 5) == 0 && token->next)
		{
			tmp = tmp->next;
			while (tmp)
			{
				if (check_unset(token->next->data) != 0)
					printf("bash: unset: '%s': not a valid identifier\n",
						token->next->data);
				tmp = tmp->next;
			}
			return (0);
		}
	}
	return (1);
}
