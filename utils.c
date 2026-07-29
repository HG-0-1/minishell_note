/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helfayez <helfayez@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/25 14:11:15 by helfayez          #+#    #+#             */
/*   Updated: 2026/07/28 15:21:09 by helfayez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	print_status(t_philo *philo, char *status)
{
	pthread_mutex_lock(&philo->data->print_lock);
	if (!get_dead(philo->data))
		printf("%ld %d %s\n", get_time() - philo->data->start_time, philo->id,
			status);
	pthread_mutex_unlock(&philo->data->print_lock);
}

int	is_valid_number(const char *str)
{
	int	i;

	i = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	ft_atoi(const char *nptr)
{
	int	i;
	int	sign;
	int	res;

	i = 0;
	sign = 1;
	res = 0;
	while (nptr[i] == ' ' || (nptr[i] >= 9 && nptr[i] <= 13))
		i++;
	if (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i] == '-')
			sign = -1;
		i++;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		res = res * 10 + (nptr[i] - '0');
		i++;
	}
	return (res * sign);
}

void	smart_sleep(long time, t_philo *philo)
{
	long	start;
	long	remaining;

	start = get_time();
	while (!get_dead(philo->data))
	{
		remaining = time - (get_time() - start);
		if (remaining <= 0)
			break ;
		if (remaining > 5)
			usleep(1000);
		else
			usleep(remaining * 1000 / 2 + 100);
	}
}
