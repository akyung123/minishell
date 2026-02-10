/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_pwd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 15:34:11 by akkim             #+#    #+#             */
/*   Updated: 2026/02/07 18:53:33 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// 현재 프로세스가 작업 중인 디렉토리의 ""절대 경로"" 출력
// getcwd() 사용

void	mini_pwd(t_info_env *env)
{
	char	*path;

	path = getcwd(NULL, 0);
	if (!path)
	{
		perror("pwd");
		env->exit_code = 1;
	}
	ft_printf("%s", path);
	free(path);
}
