/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sandbox.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpedro-f <jpedro-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 15:40:21 by jpedro-f          #+#    #+#             */
/*   Updated: 2025/07/21 16:35:57 by jpedro-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

void alarm_handler (int sig)
{
	(void)sig;
}

int	sandbox(void (*f)(void), unsigned int  timeout, bool verbose)
{
	struct sigaction	sa;
	pid_t				pid;
	int					status;

	sa.sa_handler = alarm_handler;
	sa.sa_flags = 0;
	if  (sigaction(SIGALRM, &sa, 0) < 0)
		return (-1);
	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
	{
		f();
		exit(0);
	}
	alarm(timeout);
	if (waitpid(pid, &status, 0) == -1)
	{
		if (errno ==  EINTR)
		{
			kill(pid, SIGKILL);
			waitpid(pid, NULL, 0);
			if (verbose)
				printf("Bad function: timed out after %d seconds\n", timeout);
			return (0);
		}
		return (-1);
	}
	if (WIFEXITED(status))
	{
		if (WEXITSTATUS(status) == 0)
		{
			if (verbose)
				printf("Nice function\n");
		}
		else
		{
			if (verbose)
				printf("Bad funtion: exited  with code %d\n", WEXITSTATUS(status));
			return (0);
		}
	}
	if (WIFSIGNALED(status))
	{
		if (verbose)
			printf("bad function: %s\n", strsignal(WTERMSIG(status)));
		return (0);
	}
	return (-1);
}

/*
void nice_function(void)
{
	return ;
}

void bad_ft_exit_code(void)
{
	exit(1);
}

void bad_ft_segfault(void)
{
	char *str = NULL;

	str[1] = 'h';
}

void bad_ft_timeout (void)
{
	while (1)
	{

	}
}

void bad_ft_sigkill(void)
{
	sleep(5);
}

int	main(void)
{
	int res;

	printf("Test 1: Nice function\n");
	res = sandbox(nice_function, 5, true);
	printf("Result: %d\n", res);

	printf("Test 2: Bad function due to exit code\n");
	res = sandbox(bad_ft_exit_code, 5, true);
	printf("Result: %d\n", res);

	printf("Test 3: Bad function due to segfault\n");
	res = sandbox(bad_ft_segfault, 5, true);
	printf("Result: %d\n", res);

	printf("Test 4: Bad function due to timeout\n");
	res = sandbox(bad_ft_timeout, 2, true);
	printf("Result: %d\n", res);

	printf("Test 5: Bad function killed by SIGKILL\n");
	res = sandbox(bad_ft_sigkill, 3, true);
	printf("Result: %d\n", res);

	return (0);
} 
*/