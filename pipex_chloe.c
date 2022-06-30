/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eruellan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/05 15:02:56 by eruellan          #+#    #+#             */
/*   Updated: 2022/06/21 12:19:52 by cdaveux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./minishell.h"

void	clear_elem(t_heads **final_line)
{
	t_heads	*next;

	next = NULL;
	next = (*final_line)->next;
	free_elem_heads(final_line);
	(*final_line) = next;
}

int	child(t_data *data, t_heads **line)
{
	check_heredoc(line, data);
	if (check_outfile(line) || check_infile(line))
		return (-1);
	dup2(data->tmp_fd, STDIN_FILENO);
	close(data->tmp_fd);
	event_ctrl_c(2);
	ft_exec((*line)->cmd, data);
	exit(EXIT_FAILURE); //whyyy
	return (0);
}

void	parent(t_data *data, t_heads **line)
{
	int		status;

	status = 0;
	event_ctrl_c(3);
	close (data->tmp_fd);
	if (!(*line)->next) //exec dans stdout
	{
		while (waitpid(data->pid1, &status, 0) != -1)
		{
			if (WTERMSIG(status) == 3)
			{
				g_global = 2;
				printf("Quit (core dumped)\n");
			}
			if (WIFEXITED(status) || WIFSIGNALED(status))
				return ;
		}
		data->tmp_fd = dup(STDIN_FILENO); //dont know why
	}
	else //pipe
	{
		close(data->pipe_fd[1]);
		data->tmp_fd = data->pipe_fd[0]; //save input for next cmd
	}
}

int	ft_pipex(t_data *data, t_heads **final_line, int i)
{
	t_heads	**tmp;

	(void)i;
	data->tmp_fd = dup(STDIN_FILENO);
	if (!(*final_line)->cmd)
		return (check_outfile_bis(final_line));
	if (check_infile_bis(final_line) == 1)
		return (1);
	tmp = final_line;
	while ((*final_line))
	{
		if ((*final_line)->next)
			pipe(data->pipe_fd);
		//gerer les builtins ici
		data->pid1 = fork();
		if (data->pid1 == 0)
		{
			if ((*final_line)->next) // exec dans pipe et pas dans stdout
			{
				dup2(data->pipe_fd[1], STDOUT_FILENO);
				close(data->pipe_fd[0]);
				close(data->pipe_fd[1]);
			}
			if (child(data, final_line) == -1)
				return (clear_all_heads(final_line));
		}
		else
			parent(data, final_line);
		clear_elem(&(*final_line));
	}
	close(data->tmp_fd);
//	close_fds(data);
	return (0);
}

int	ft_exit(t_heads **line)
{
	int		i;
	int		tmp;
	t_token	*cmd;

	i = -1;
	cmd = (*line)->cmd;
	while (cmd->next->data[++i])
	{
		if (i == 0 && cmd->next->data[i] == '-')
			i++;
		if (!ft_isdigit(cmd->next->data[i]))
			return (ft_exit_message(1, line));
	}
	if (cmd->next->next)
		return (ft_exit_message(2, line));
	if (ft_atoi(cmd->next->data) < 0)
	{
		clear_all_heads(line);
		return (255);
	}
	tmp = ft_atoi(cmd->next->data) % 256;
	clear_all_heads(line);
	return (tmp);
}