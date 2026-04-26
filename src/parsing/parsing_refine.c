/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_refine.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/26 16:41:42 by akkim             #+#    #+#             */
/*   Updated: 2026/04/26 17:09:32 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quote.h"
#include "parsing.h"
#include "minishell.h"

char *append_char(char *str, char c)
{
	char	*new_str;
	int		len;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	new_str = malloc(len + 2);
	ft_strlcpy(new_str, str, len + 1);
	new_str[len] = c;
	new_str[len + 1] = '\0';
	free(str);
	return (new_str);
}

char *ft_strjoin_free(char *s1, char *s2)
{
	char	*new_str;

	if (!s1 || !s2)
		return (NULL);
	new_str = ft_strjoin(s1, s2);
	free(s1);
	return (new_str);
}

// 환경변수 변환 함수
	// 변환 시, ' 안에는 변환 X
	//			" 안에는 변환되어야함
	// (이 때는 $USER와 같은 경우)
	// 변수명으로 환경변수 값을 찾아주는 가상의 헬퍼 함수 (이미 구현되어 있다면 그것을 사용하세요!)
// 만약 못 찾으면 빈 문자열("") 또는 NULL을 반환해야 합니다.

void	expand_env(char **line, t_info_env *env)
{
	char	*res;
	char	*var_name;
	char	*var_value;
	int		i;
	int		start;
	int		state;

	if (!line || !(*line))
		return ;

	res = ft_strdup(""); // 빈 문자열로 시작
	i = 0;
	state = 0;
	while ((*line)[i])
	{
		update_quote_state((*line)[i], &state);

		if ((*line)[i] == '$' && !(state & 1))
		{
			i++;
			if ((*line)[i] == '?')
			{
				var_value = ft_itoa(env->exit_code);
				res = ft_strjoin_free(res, var_value);
				free(var_value);
				i++;
			}
			else if (ft_isalnum((*line)[i]) || (*line)[i] == '_')
			{
				start = i;
				while ((*line)[i] && (ft_isalnum((*line)[i]) || (*line)[i] == '_'))
					i++;
				var_name = ft_substr(*line, start, i - start);
				var_value = get_env_val(env->head, var_name);
				if (var_value)
					res = ft_strjoin_free(res, var_value);
				free(var_name);
			}
			else
				res = append_char(res, '$'); 
		}
		else
		{
			res = append_char(res, (*line)[i]);
			i++;
		}
	}
	free(*line);
	*line = res;
}

// 따옴표 제거 함수
void	remove_quotes(char *line)
{
	int		i;
	int		j;
	int		state;
	int		prev_state;

	if (!line)
		return ;
	i = 0;
	j = 0;
	state = 0;
	while (line[i])
	{
		prev_state = state;
		update_quote_state(line[i], &state);
		if (state != prev_state)
		{
			i++;
			continue ;
		}
		line[j++] = line[i++];
	}
	line[j] = '\0';
}

// quote 제거, 환경변수 제거 함수
void	refine_line(char **line, t_info_env *env)
{
	if (!line || !(*line))
		return ;
	expand_env(line, env);
	if (*line) 
		remove_quotes(*line);
}