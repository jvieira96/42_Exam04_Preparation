/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_popen.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpedro-f <jpedro-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 12:59:39 by jpedro-f          #+#    #+#             */
/*   Updated: 2025/07/21 13:27:39 by jpedro-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>

int ft_popen(const char *file, char *const argv[], char type)
{
    int fd[2];
    int pid;

    if (!file || !argv || (type != 'r' && type != 'w'))
        return (-1);
    if (pipe(fd) == -1)
        return (-1);
    pid = fork();
    if (pid == -1)
    {
        close(fd[0]);
        close (fd[1]);
        return (-1);
    }
    if (pid == 0)
    {
        if (type == 'r')
        {
            if (dup2(fd[1], 1) == -1)
                exit(1);
        }
        if (type == 'w')
        {
            if (dup2(fd[0], 0) == -1)
                exit (1);
        }
        close(fd[0]);
        close(fd[1]);
        execvp(file, argv);
        exit(1);
    }
    if (type == 'r')
    {
        close(fd[1]);
        return (fd[0]);
    }
    else
    {
        close(fd[0]);
        return (fd[1]);
    }
}

/* 
int main ()
{
    int fd_ls;
    int fd_wc;
    char buffer[1024];
    int bytes_read;

    fd_ls = ft_popen("ls", (char *const[]){"ls", NULL}, 'r');
    fd_wc = ft_popen("wc", (char *const[]){"wc", "-w", NULL}, 'w');
    if (fd_ls == -1 || fd_wc == -1)
        return (1);
    while ((bytes_read = read(fd_ls, buffer, sizeof(buffer))) > 0)
        write(fd_wc, buffer, bytes_read);
    close(fd_wc);
    close(fd_ls);
    return (0);
}
 */