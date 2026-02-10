/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 17:26:48 by akkim             #+#    #+#             */
/*   Updated: 2026/02/10 19:01:52 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
// minishell을 위한 env setting 함수 모음
// env는 연결 리스트 형태로 만듬

// 노드 생성
t_env	*new_env_node(char *key, char *value)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = ft_strdup(key);
	if (value)
		node->value = ft_strdup(value);
	else
		node->value = NULL;
	node->next = NULL;
	return (node);
}

// 노드 추가
void	env_add_back(t_env **head, t_env *new)
{
	t_env	*curr;

	if (!*head)
	{
		*head = new;
		return ;
	}
	curr = *head;
	while (curr->next)
		curr = curr->next;
	curr->next = new;
}

// 초기화
// main에서 호출할 함수
void	init_env(t_info_env *env, char **envp)
{
	int		i;
	char	*eq;
	char	*key;
	char	*value;

	i = 0;
	env->head = NULL;
	while (envp[i])
	{
		// '=' 위치 찾기
		eq = ft_strchr(envp[i], '=');
		if (eq)
		{
			// 앞은 key, 뒤는 value로 자르기
			key = ft_substr(envp[i], 0, eq - envp[i]);
			value = ft_strdup(eq + 1);
			env_add_back(&env->head, new_env_node(key, value));
			free(key);
			free(value);
		}
		i++;
	}
}

// key로 노드 찾기
t_env	*find_env_node(t_env *head, char *key)
{
	while (head)
	{
		if (ft_strlen(head->key) == ft_strlen(key) && \
			ft_strncmp(head->key, key, ft_strlen(key)) == 0)
			return (head);
		head = head->next;
	}
	return (NULL);
}

// 값 가져오기
char	*get_env_val(t_env *head, char *key)
{
	t_env	*node;

	node = find_env_node(head, key);
	if (node)
		return (node->value);
	return (NULL);
}

// 환경변수 추가

// 삭제