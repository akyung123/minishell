/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 16:28:09 by akkim             #+#    #+#             */
/*   Updated: 2026/05/02 04:44:41 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include "ft_printf_bonus.h"
# include "get_next_line.h"
# include <stdio.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>
# include <signal.h>
# include <termios.h>

typedef struct l_pipex t_pipex;

typedef struct l_env
{
	char			*key;
	char			*value;
	struct l_env	*next;
}	t_env;

typedef struct l_info_env
{
	t_env	*head;
	t_env	*hide_head;
	char	**envp;
	int		exit_code;
	t_pipex	*pipex;
}		t_info_env;

extern int	g_signo;

void	init_env(t_info_env *env, char **envp);
char	*get_env_val(t_env *head, char *key);
char	*get_env_val_all(t_info_env *env, char *key);

// built-in function
void	mini_pwd(t_info_env *env);
void	mini_env(t_info_env *env);
void	mini_export(t_info_env *env, char **args);
void	mini_unset(t_info_env *env, char **args);
void	mini_cd(t_info_env *env, char *path);
void	mini_exit(t_info_env *env, char **code);
void	mini_echo(t_info_env *env, char **str);

int		is_builtin(char *cmd);

void	set_terminal_print_off(void);
void	set_signal(void);

void	update_envp_array(t_info_env *env);
void	update_last_arg(t_info_env *env, char *last_arg);

void	shell_cleanup(t_info_env *env);

#endif