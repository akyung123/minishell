/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_simple_build_cmd.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 02:53:27 by akkim             #+#    #+#             */
/*   Updated: 2026/04/27 07:28:01 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quote.h"

// 간단한 리다이렉션 생성 함수
static t_redirect	*create_redir(char *type, char *file)
{
	t_redirect	*new;

	new = malloc(sizeof(t_redirect));
	if (!new)
		return (NULL);
	new->type = ft_strdup(type);
	new->filename = ft_strdup(file);
	return (new);
}

// 따옴표(껍데기)만 제거하는 전용 함수
static void	remove_quotes_only(char **line)
{
	char	*str;
	char	*new;
	int		i[2];
	int		state;
	int		prev;

	if (!line || !*line)
		return ;
	str = *line;
	new = malloc(ft_strlen(str) + 1);
	if (!new)
		return ;
	i[0] = -1;
	i[1] = 0;
	state = 0;
	while (str[++i[0]])
	{
		prev = state;
		update_quote_state(str[i[0]], &state);
		if (state == prev)
			new[i[1]++] = str[i[0]];
	}
	new[i[1]] = '\0';
	free(str);
	*line = new;
}

static int	count_args(char **tokens)
{
	int	i;
	int	count;

	if (!tokens)
		return (0);
	i = 0;
	count = 0;
	while (tokens[i])
	{
		if (is_redir(tokens[i]))
		{
			if (tokens[i + 1])
				i += 2;
			else
				i++;
		}
		else
		{
			count++;
			i++;
		}
	}
	return (count);
}

static void	handle_redir_tokens(t_simple_command *cmd,
	char **tokens, int *i, t_info_env *env)
{
	if (ft_strcmp(tokens[*i], "<<") == 0)
	{
		remove_quotes_only(&tokens[*i + 1]);
		cmd->pre_red = create_redir(tokens[*i], tokens[*i + 1]);
	}
	else
	{
		refine_line(&tokens[*i + 1], env);
		if (ft_strcmp(tokens[*i], "<") == 0)
			cmd->pre_red = create_redir(tokens[*i], tokens[*i + 1]);
		else if (ft_strcmp(tokens[*i], ">") == 0
			|| ft_strcmp(tokens[*i], ">>") == 0)
			cmd->suff_red = create_redir(tokens[*i], tokens[*i + 1]);
	}
	*i += 2;
}

t_simple_command	*build_cmd_struct(char **tokens, t_info_env *env)
{
	t_simple_command	*cmd;
	int					i;
	int					j;

	cmd = ft_calloc(1, sizeof(t_simple_command));
	if (!cmd)
		return (NULL);
	cmd->args = malloc(sizeof(char *) * (count_args(tokens) + 1));
	i = 0;
	j = 0;
	while (tokens[i])
	{
		if (is_redir(tokens[i]))
			handle_redir_tokens(cmd, tokens, &i, env);
		else
		{
			refine_line(&tokens[i], env);
			add_expanded_args(cmd, tokens[i], &j);
			i++;
		}
	}
	cmd->args[j] = NULL;
	if (j > 0)
		cmd->cmd = ft_strdup(cmd->args[0]);
	return (cmd);
}
