/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sandbox.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpedro-f <jpedro-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 15:40:21 by jpedro-f          #+#    #+#             */
/*   Updated: 2025/07/23 17:09:28 by jpedro-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>
#include <errno.h>

void do_nothing(int sig)
{
	(void)sig;
}

int	sandbox(void (*f)(void), unsigned int timeout, bool verbose)
{
	struct sigaction	sa;
	pid_t				pid;
	int					status;
	
	sa.sa_handler = do_nothing;
	sa.sa_flags = 0;
	if (sigaction(SIGALRM, &sa, 0) < 0)
		return -1;
	pid = fork();
	if (pid == 0)
	{
		f();
		exit(0);
	}
	alarm(timeout);
	if (waitpid(pid, &status, 0) == -1)
	{
		if (errno == EINTR)
		{
			kill(pid, SIGKILL);
			waitpid(pid, NULL, 0);
			if (verbose)
				printf("Bad function: timed out after %d seconds\n", timeout);
			return 0;
		}
	}
	if (WIFEXITED(status))
	{
		if (WEXITSTATUS(status) == 0)
		{
			if (verbose)
				printf("Nice function!\n");
			return (1);
		}
		else
		{
			if (verbose)
				printf("Bad funtion: exited  with code %d\n", WEXITSTATUS(status));
			return 0;
		}
	}
	if (WIFSIGNALED(status))
	{
		if (verbose)
			printf("bad function: %s\n", strsignal(WTERMSIG(status)));
		return 0;
	}
	return -1;
}

/* 
void nice_funtion(void)
{
	return ;
}

void bad_exit_code(void)
{
	exit(1);
}

void bad_segfault(void)
{
	char *str = NULL;

	str[1] = 'a';
}

void bad_timeout(void)
{
	while (1)
	{

	}
}

void bad_sigkill(void)
{
	sleep(10);
}



int main(void)
{
	int res;

	printf("Nice function:\n");
	res = sandbox(nice_funtion, 5, true);
	printf("result: %d\n", res);

	printf("bad function exit code\n");
	res = sandbox(bad_exit_code, 5, true);
	printf("result: %d\n", res);

	printf("bad function segfault\n");
	res = sandbox(bad_segfault, 5, true);
	printf("result: %d\n", res);

	printf("bad function killed by sigkill \n");
	res = sandbox(bad_sigkill, 5, true);
	printf("result: %d\n", res);

	printf("bad function timeout\n");
	res = sandbox(bad_timeout, 3, true);
	printf("result: %d\n", res);
	return 0;
}
 */