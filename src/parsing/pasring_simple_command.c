/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pasring_simple_command.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 12:59:11 by akkim             #+#    #+#             */
/*   Updated: 2026/04/27 00:25:45 by akkim            ###   ########.fr       */
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

// 따옴표(껍데기)만 제거하는 전용 함수
void	remove_quotes_only(char **line)
{
	char	*str;
	char	*new;
	int		i;
	int		j;
	int		state;
	int		prev;

	if (!line || !*line)
		return ;
	str = *line;
	new = malloc(ft_strlen(str) + 1);
	if (!new)
		return ;
	i = -1;
	j = 0;
	state = 0;
	while (str[++i])
	{
		prev = state;
		update_quote_state(str[i], &state);
		if (state != prev)
			continue ;
		new[j++] = str[i];
	}
	new[j] = '\0';
	free(str);
	*line = new;
}

void	add_expanded_args(t_simple_command *cmd, char *token, int *j)
{
	char	**split;
	char	**new_args;
	int		k;
	int		len;

	split = ft_split(token, ' ');
	len = 0;
	while (split && split[len])
		len++;
	new_args = ft_calloc(*j + len + 1, sizeof(char *)); // 기존 개수 + 새 개수 + NULL
	k = -1;
	while (++k < *j)
		new_args[k] = cmd->args[k]; // 기존 인자들 복사
	k = -1;
	while (++k < len)
	{
		if (*j == 0 && k == 0)
			cmd->cmd = ft_strdup(split[k]); // 첫 번째 단어는 cmd 이름으로
		new_args[*j + k] = ft_strdup(split[k]);
	}
	free(cmd->args);
	cmd->args = new_args;
	*j += len; // 인덱스 j를 추가된 개수만큼 증가시킴
	free_tokens(split); // 임시로 쪼갠 껍데기 배열 해제 (현재 파일에 있는 함수)
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
		{
			if (ft_strcmp(tokens[i], "<<") == 0)
			{
				remove_quotes_only(&tokens[i + 1]);
				cmd->pre_red = create_redir(tokens[i], tokens[i + 1]);
			}
			else
			{
				refine_line(&tokens[i + 1], env);
				ft_printf("%s", tokens[i+1]);
				if (ft_strcmp(tokens[i], "<") == 0)
					cmd->pre_red = create_redir(tokens[i], tokens[i + 1]);
				else if (ft_strcmp(tokens[i], ">") == 0 || ft_strcmp(tokens[i], ">>") == 0)
					cmd->suff_red = create_redir(tokens[i], tokens[i + 1]);
			}
			i += 2;
		}
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
	simple_command = build_cmd_struct(tokens, env);
	free_tokens(tokens);
	return (simple_command);
}
