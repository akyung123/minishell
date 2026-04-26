/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pasring_simple_command.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 12:59:11 by akkim             #+#    #+#             */
/*   Updated: 2026/04/26 18:02:57 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	free_tokens(char **tokens)
{
	int	i;

	if (!tokens)
		return ;
	i = 0;
	while (tokens[i])
	{
		free(tokens[i]);
		i++;
	}
	free(tokens);
}

// redirect 찾기
int	is_redir(char *token)
{
	if (ft_strcmp(token, ">") == 0 || ft_strcmp(token, "<") == 0 ||
		ft_strcmp(token, ">>") == 0 || ft_strcmp(token, "<<") == 0)
		return (1);
	return (0);
}

// 문법 체크 함수
// 파일명 x && 기호 뒤에 기호 오는 경우
// $는 변경하고 `(백틱)은 실행시켜야함
int	check_syntax(char **tokens)
{
	int	i;

	i = 0;
	if (!tokens[0])
		return (0);
	while (tokens[i])
	{
		if (is_redir(tokens[i]))
		{
			if (tokens[i + 1] == NULL)
				return (0);
			else if (is_redir(tokens[i + 1]))
				return (0);
			i += 2;
		}
		else
			i++;
	}
	return (1);
}

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

// 리다이렉션 해제 함수
/*
static void	free_redir(t_redirect *redir)
{
	if (!redir)
		return ;
	free(redir->type);
	free(redir->filename);
	free(redir);
}
*/

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

t_simple_command	*build_cmd_struct(char **tokens)
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
		{
			if (ft_strcmp(tokens[i], "<") == 0 || ft_strcmp(tokens[i], "<<") == 0)
				cmd->pre_red = create_redir(tokens[i], tokens[i + 1]);
			else if (ft_strcmp(tokens[i], ">") == 0 || ft_strcmp(tokens[i], ">>") == 0)
				cmd->suff_red = create_redir(tokens[i], tokens[i + 1]);
			i += 2;
		}
		else
		{
			if (j == 0)
				cmd->cmd = ft_strdup(tokens[i]);
			cmd->args[j++] = ft_strdup(tokens[i++]);
		}
	}
	cmd->args[j] = NULL;
	if (j > 0)
		cmd->cmd = ft_strdup(cmd->args[0]);
	return (cmd);
}

// ' '를 추가 후, 토큰화 -> 문법 검사 필요!
// <simple_command> ::= <cmd_name> <arguments_opt> <redirects_opt>
t_simple_command	*parsing_simple_command(t_info_env *env, char *line)
{
	t_simple_command	*simple_command;
	char				**tokens;

	(void)env;
	if (!line)
		return (NULL);
	tokens = tokenize_line(line);
	if (!tokens)
		return (NULL);
	if (!check_syntax(tokens))
	{
		free_tokens(tokens);
		return (NULL);
	}
	simple_command = build_cmd_struct(tokens);
	free_tokens(tokens);
	return (simple_command);
}
