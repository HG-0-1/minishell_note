/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_and_routine.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helfayez <helfayez@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/28 12:06:45 by helfayez          #+#    #+#             */
/*   Updated: 2026/07/29 01:36:34 by helfayez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	one_philo(t_philo *philo)
{
	if (philo->data->num_philo == 1)
	{
		fork_lock(philo->left_fork, philo->data);
		print_status(philo, "has taken a fork");
		usleep(philo->data->time_to_die * 1000);
		fork_unlock(philo->left_fork, philo->data);
		return (1);
	}
	return (0);
}

void	*monitor(void *arg)
{
	t_data	*data;
	int		i;

	data = arg;
	while (!get_dead(data))
	{
		if (all_ate_enough(data))
		{
			pthread_mutex_lock(&data->dead_lock);
			data->dead = 1;
			pthread_mutex_unlock(&data->dead_lock);
			return (NULL);
		}
		i = 0;
		while (i < data->num_philo)
		{
			if (check_death(&data->philo[i]))
				return (NULL);
			i++;
		}
		usleep(1000);
	}
	return (NULL);
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = arg;
	pthread_mutex_lock(&philo->meal_lock);
	philo->last_meal = get_time();
	pthread_mutex_unlock(&philo->meal_lock);
	if (one_philo(philo))
		return (NULL);
	while (!get_dead(philo->data))
	{
		print_status(philo, "is thinking");
		if (take_forks(philo))
			break ;
		eat(philo);
		if (philo->data->must_eat > 0
			&& philo->meal_eaten >= philo->data->must_eat)
			break ;
		print_status(philo, "is sleeping");
		smart_sleep(philo->data->time_to_sleep, philo);
	}
	return (NULL);
}
