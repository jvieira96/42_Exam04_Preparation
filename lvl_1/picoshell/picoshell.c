/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_picoshell.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpedro-f <jpedro-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 13:34:55 by jpedro-f          #+#    #+#             */
/*   Updated: 2025/07/21 14:58:23 by jpedro-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int picoshell(char **cmds[])
{
	int 	fd[2];
	pid_t 	pid;
	int		i = 0;
	int		fd_stdin = 0;
	int		status = 0;
	int		res = 0;

	while (cmds[i])
	{
		if (cmds[i + 1])
		{
			if (pipe(fd) == -1)
				return (1);
		}
		else
		{
			fd[0] = -1;
			fd[1] = -1;
		}
		pid = fork();
		if (pid == -1)
		{
			if (fd[0] != -1)
				close(fd[0]);
			if (fd[1] != -1)
				close(fd[1]);
			if (fd_stdin != 0)
				close(fd_stdin);
			return (1);
		}
		if (pid == 0)
		{
			if (fd_stdin != 0)
			{
				if (dup2(fd_stdin, 0) == -1)
					exit (1);
				close (fd_stdin);
			}
			if (fd[1] != -1)
			{
				if (dup2(fd[1], 1) == -1)
					exit(1);
				close(fd[1]);
				close(fd[0]);
			}
			execvp(cmds[i][0], cmds[i]);
			exit(1);
		}
		if (fd_stdin != 0)
			close(fd_stdin);
		if (fd[1] != -1)  
			close(fd[1]);
		fd_stdin = fd[0];
		i++;
	}
	while(wait (&status) > 0)
	{
		if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
			res = 1;
		else if (!WIFEXITED(status))
			res = 1;
	}
	return (res);
}


int main()
{
	
/* 	char *cmd1[] = {"ls", NULL};
	char *cmd2[] = {"grep", "picoshell", NULL};
	char **cmds[] = {cmd1, cmd2, NULL}; */
	

	char *cmd1[] = {"echo", "squalala", NULL};
	char *cmd2[] = {"cat", NULL};
	char *cmd3[] = {"sed", "s/a/b/g", NULL};
	char **cmds[] = {cmd1, cmd2, cmd3, NULL};
	return (picoshell(cmds));
}
