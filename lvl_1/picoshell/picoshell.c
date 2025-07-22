/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   picoshell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpedro-f <jpedro-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 13:34:55 by jpedro-f          #+#    #+#             */
/*   Updated: 2025/07/22 19:07:16 by jpedro-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int    picoshell(char **cmds[])
{
	int	i = -1;
	int	fd[2];
	pid_t	pid;
	int fd_stdin = 0;
	int status = 0;
	int res = 0;

	while (cmds[++i])
	{
		if (cmds[i + 1])
			pipe(fd);
		else
		{
			fd[0] = -1;
			fd[1] = -1;
		}
		pid = fork();
		if (pid == 0)
		{
			if (fd_stdin != 0)
			{
				dup2(fd_stdin, 0);
				close(fd_stdin);
			}
			if (fd[1] != -1)
			{
				dup2(fd[1], 1);
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
	}
	while (wait(&status) > 0)
	{
		if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
			res = 1;
		else if (!WIFEXITED(status))
			res = 1;
	}
	return (res);
}
/* 
int main()
{
	char *cmd1[] = {"ls", NULL};
	char *cmd2[] = {"wc", "-w", NULL};
	char **cmds[] = {cmd1, cmd2, NULL};

	return(picoshell(cmds));
} */