/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/03 17:13:05 by akkim             #+#    #+#             */
/*   Updated: 2026/04/15 21:10:43 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

#include <fcntl.h>

typedef struct l_pipex
{
	int		pid;
	int		in;
	int		out;
	int		fd[2];
	char	**paths;
	char	**envp;
}	t_pipex;

pid_t	run_process(t_pipex *piepx, t_simple_command *simple_command);

#endif