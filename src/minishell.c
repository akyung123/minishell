/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 15:02:03 by akkim             #+#    #+#             */
/*   Updated: 2026/04/26 22:32:36 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "env.h"
#include "parsing.h"
#include <stdio.h>

int	is_only_whitespace(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		// 9-13번은 탭, 개행 등 / 32번은 스페이스
		if (!((line[i] >= 9 && line[i] <= 13) || line[i] == 32))
			return (0); // 공백이 아닌 문자가 발견됨
		i++;
	}
	return (1); // 끝까지 공백만 있음
}

int	main(int argc, char **argv, char **envp)
{
	t_info_env		env;
	char			*line;
	t_command_line	*command_line;

	(void)argc;
	(void)argv;
	init_env(&env, envp);
	while (1)
	{
		line = readline("minishell: ");
		if (!line)
			break ;
		if (is_only_whitespace(line))
		{
			free(line);
			continue ;
		}
        if (*line != '\0')
            add_history(line);
		command_line = parsing_command_line(&env, &line);
        if (command_line)
		    executor_command_line(&env, command_line);
		// test_command_line(command_line);
		free_command_line(command_line);
		free(line);
	}
    // line 구조체를 위한 free() 함수가 필요함
	all_free_env(env.head);
}

/*
// 테스트 실행 함수
void	run_parsing_tests(void)
{
    t_command_line	*cmd;
    char			*test_cases[] = {
        // 기본 파이프
        "ls | grep test",
        
        // 복잡한 파이프 (3개 이상)
        "cat file.txt | grep pattern | wc -l",
        "ls -la | grep '.c' | sort | head -5",
        
        // 리다이렉션 + 파이프
        "cat < input.txt | grep test > output.txt",
        "ls -la >> log.txt | grep error",
        
        // 명령어 조합 (&&, ||)
        "ls | grep test && echo found",
        "cat missing.txt || echo 'file not found'",
        "ls && grep test && wc -l",
        
        // 복잡한 조합
        "cat file.txt | grep error > errors.log && echo done",
        "find . -name '*.c' | xargs grep main || echo 'not found'",
        
        // 따옴표와 특수문자
        "echo 'hello world' | grep 'hello'",
        "ls | grep \"test file\"",
        
        // 변수 확장
        "echo $HOME | grep home",
        "ls $PATH | grep bin",
        
        // 복합 복잡도
        "cat file1.txt file2.txt | sort | uniq -c | sort -rn | head -10",
        NULL
    };
    
    int i = 0;
    while (test_cases[i])
    {
        printf("\n========================================\n");
        printf("INPUT: %s\n", test_cases[i]);
        printf("========================================\n");
        
        cmd = parse_command_line(test_cases[i]);
        test_command_line(cmd);
        
        // 메모리 해제
        free_command_line(cmd);
        i++;
    }
}*/