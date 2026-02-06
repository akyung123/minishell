/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_echo.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 16:59:15 by akkim             #+#    #+#             */
/*   Updated: 2026/02/06 17:00:33 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// echo 명령어 구현
// $USER $HOEM과 같은 변수 출력 가능함

// 필요 변수
// - 출력 문자열
// - n 옵션 여부(flag)
// - 환경변수가 필요함
// - 종료코드를 위한 구조체

static char	**find_env(char *str, char **envp)
{
	int		i;
	char	**paths;

	i = 1;
	while (ft_strcmp(envp[i], str) != 0)
		i++;
	// "\n"찾아서 끊기
	return (paths);
}

// 추후, env & 종료코드 & flag를 담은 구조체를 만들어 넘기는 걸 추천함
void mini_echo(char *str, char **env, int flag)
{
	if (ft_strncmp(str, "$", 1))
	{
		// 다음 $까지 생각해봐야함
		find_env(str,)
	}
}