/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_simple_build_cmd.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 02:53:27 by akkim             #+#    #+#             */
/*   Updated: 2026/05/03 12:07:23 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quote.h"
#include "env.h"

// 간단한 리다이렉션 생성 함수
static t_redirect	*create_redir(char *type, char *file)
{
	t_redirect	*new;

	new = ft_calloc(1, sizeof(t_redirect));
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
		if (str[i[0]] == '\\' && str[i[0] + 1] && state != 1)
		{
			i[0]++;
			new[i[1]++] = str[i[0]];
			continue ;
		}
		prev = state;
		update_quote_state(str[i[0]], &state);
		if (state == prev)
			new[i[1]++] = str[i[0]];
	}
	new[i[1]] = '\0';
	free(str);
	*line = new;
}

static t_redirect	*handle_redir_tokens(char **tokens, int *i, t_info_env *env)
{
	t_redirect	*red;
	int			flag;

	flag = 1;
	if (ft_strcmp(tokens[*i], "<<") == 0)
	{
		if (ft_strchr(tokens[*i + 1], '\'') || ft_strchr(tokens[*i + 1], '\"'))
			flag = 0;
		remove_quotes_only(&tokens[*i + 1]);
		red = create_redir(tokens[*i], tokens[*i + 1]);
		red->flag = flag;
	}
	else
	{
		refine_line(&tokens[*i + 1], env);
		if (ft_strcmp(tokens[*i], "<") == 0)
			red = create_redir(tokens[*i], tokens[*i + 1]);
		else if (ft_strcmp(tokens[*i], ">") == 0
			|| ft_strcmp(tokens[*i], ">>") == 0)
			red = create_redir(tokens[*i], tokens[*i + 1]);
	}
	*i += 2;
	return (red);
}

// maximun here_doc
static void	maximun_hd_exit(t_info_env *env, char **tokens)
{
	free_tokens(tokens);
	all_free_env(env->head);
	ft_putstr_fd("bash: maximum here-document count exceeded", 2);
	exit(2);
}

void	handle_redirection(int i, )
{
	if (i[2]++ >= 16)
		maximun_hd_exit(env, tokens);
}

// i[0]=tokens, i[1]=args(j), i[2]=red_num
t_simple_command	*build_cmd_struct(char **tokens, t_info_env *env)
{
	t_simple_command	*cmd;
	t_redirect			*red_tmp;
	t_redirect			*new;
	char				**split_words;
	int					i[3];
	int					k;

	cmd = ft_calloc(1, sizeof(t_simple_command));
	if (!cmd)
		return (NULL);
	cmd->args = ft_calloc(1024, sizeof(char *));
	ft_memset(i, 0, sizeof(int) * 3);
	while (tokens[i[0]])
	{
		if (is_redir(tokens[i[0]]))
		{
			if (i[2]++ >= 16)
				maximun_hd_exit(env, tokens);
			new = handle_redir_tokens(tokens, &i[0], env);
			if (new)
			{
				if (!cmd->red)
					cmd->red = new;
				else
					red_tmp->next = new;
				red_tmp = new;
			}
		}
		else
		{
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
	}
	return (cmd);
}
