/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 18:00:44 by akkim             #+#    #+#             */
/*   Updated: 2026/02/28 00:01:10 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PASSING_H
# define PASSING_H

# include "minishell.h"

typedef struct l_redirect
{
	char	type;
	char	*filename;
}	t_redirect;

typedef struct l_simple_command
{
	char		*cmd;
	char		**args;
	t_redirect	*pre_red;
	t_redirect	*suff_red;

}	t_simple_command;

typedef struct l_pipeline
{
	t_simple_command	*simple_command;
	struct l_pipeline	*next;
}	t_pipeline;

typedef struct l_command_line
{
	char 					comm_oper;	// '&&' or '||' or 'None'
	t_pipeline				*pipeline;	// 왼쪽 파이프 라인 **항상 존재함**
	struct l_command_line	*next;	// 오른쪽 코멘트 라인 ** 존재 or 'None'** None되면 line 종료
}	t_command_line;

char	*ft_strrchr(const char *str, int ch);
char	*ft_strstr(const char *haystack, const char *needle);

void	builin_handler(t_info_env *env, t_simple_command *simple_command);
t_command_line	*passing_command_line(char *line);

#endif