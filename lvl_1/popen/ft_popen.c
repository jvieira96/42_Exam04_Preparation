/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_popen.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpedro-f <jpedro-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 14:59:59 by jpedro-f          #+#    #+#             */
/*   Updated: 2025/07/23 15:42:50 by jpedro-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h> 
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int ft_popen(const char *file, char *const argv[], char type)
{
	int	 	fd[2];
	pid_t 	pid;

	if (!file || !argv || (type != 'r' && type != 'w'))
		return (-1);
	pipe(fd);
	pid = fork();
	if (pid == 0)
	{
		if (type == 'r')
			dup2(fd[1], 1);
		if (type == 'w')
			dup2(fd[0], 0);
		close(fd[0]);
		close(fd[1]);
		execvp(file, argv);
		exit(1);
	}
	if (type == 'r')
	{
		close(fd[1]);
		return(fd[0]);
	}
	else
	{
		close(fd[0]);
		return (fd[1]);
	}
}

/* 
//test type 'r'
int	main() 
{	
	int fd = ft_popen("ls", (char *const[]){"ls", "-l", NULL}, 'r');
	char buf[1];

	while (read(fd, buf, 1))
		write(1, buf, 1);
	close(fd);
	return (0);
}
 */

/* 
//test type 'w'
int main()
{
    int fd = ft_popen("wc", (char *const[]){"wc", "-w", NULL}, 'w');
    char *input = "Hello world\nThis is a test\nthird line mofo\n";

    write(fd, input, strlen(input));
	close(fd);
    return (0);
}
 */