/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_simple_build_cmd_util.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/03 14:20:05 by akkim             #+#    #+#             */
/*   Updated: 2026/05/03 14:22:40 by akkim            ###   ########.fr       */
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

void	handle_redirection(t_simple_command	*cmd, char **tokens,
	t_info_env *env, int *i)
{
	t_redirect			*new;
	t_redirect			*last;

	if (i[2]++ >= 16)
		maximun_hd_exit(env, tokens);
	new = handle_redir_tokens(tokens, &i[0], env);
	if (!new)
		return ;
	if (!cmd->red)
	{
		cmd->red = new;
	}
	else
	{
		last = cmd->red;
		while (last->next)
			last = last->next;
		last->next = new;
	}
}
