/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_echo.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 16:59:15 by akkim             #+#    #+#             */
/*   Updated: 2026/02/07 15:06:32 by akkim            ###   ########.fr       */
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

// 추후, env & 종료코드 & flag를 담은 구조체를 만들어 넘기는 걸 추천함
// 입력은 어떻게? - echo $USER
// $USER는 자동으로 akkim으로 변환되어서 들어와짐 -> 즉 반영안해도 됨

// char **env
void mini_echo(t_info_env *env, char **str)
{
    int j;
    int k;
    int n_flag;

    (void)env;
    j = 1;
	n_flag = 0;
    while (str[j] && str[j][0] == '-' && str[j][1] == 'n')
    {
        k = 1;
        while (str[j][k] == 'n')
            k++;
        if (str[j][k] == '\0')
        {
            n_flag = 1;
            j++;
        }
        else
            break;
    }
    while (str[j])
    {
        ft_printf("%s", str[j]);
        if (str[j + 1])
            ft_printf(" ");
        j++;
    }
    if (n_flag == 0)
        ft_printf("\n");
}
