/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_simple_build_cmd.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 02:53:27 by akkim             #+#    #+#             */
/*   Updated: 2026/05/03 15:12:20 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quote.h"
#include "env.h"

static void	fill_unquoted_str(char *new, char *str)
{
	int	i;
	int	j;
	int	st;
	int	prv;

	i = -1;
	j = 0;
	st = 0;
	while (str[++i])
	{
		if (str[i] == '\\' && str[i + 1] && st != 1 && ++i)
		{
			new[j++] = str[i];
			continue ;
		}
		prv = st;
		update_quote_state(str[i], &st);
		if (st == prv)
			new[j++] = str[i];
	}
	new[j] = '\0';
}

void	remove_quotes_only(char **line)
{
	char	*new;

	if (!line || !*line)
		return ;
	new = malloc(ft_strlen(*line) + 1);
	if (!new)
		return ;
	fill_unquoted_str(new, *line);
	free(*line);
	*line = new;
}

void	handle_argument(t_simple_command *cmd,
	char **tokens, t_info_env *env, int *i)
{
	char	**split_words;
	int		k;

	refine_line(&tokens[i[0]], env);
	split_words = ft_split(tokens[i[0]], '\x1F');
	i[0]++;
	k = 0;
	while (split_words && split_words[k])
	{
		remove_quotes_only(&split_words[k]);
		add_expanded_args(cmd, split_words[k++], &i[1]);
	}
	free_split(split_words);
}

// i[0]=tokens, i[1]=args(j), i[2]=red_num
t_simple_command	*build_cmd_struct(char **tokens, t_info_env *env)
{
	t_simple_command	*cmd;
	int					i[3];

	cmd = ft_calloc(1, sizeof(t_simple_command));
	if (!cmd)
		return (NULL);
	cmd->args = ft_calloc(1024, sizeof(char *));
	ft_memset(i, 0, sizeof(int) * 3);
	while (tokens[i[0]])
	{
		if (is_redir(tokens[i[0]]))
			handle_redirection(cmd, tokens, env, i);
		else
			handle_argument(cmd, tokens, env, i);
	}
	return (cmd);
}
