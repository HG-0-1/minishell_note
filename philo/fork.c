/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helfayez <helfayez@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/25 12:32:53 by helfayez          #+#    #+#             */
/*   Updated: 2026/07/28 15:14:39 by helfayez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	fork_lock(t_fork *fork, t_data *data)
{
	pthread_mutex_lock(&data->table_lock);
	fork->in_use = 1;
	pthread_mutex_unlock(&data->table_lock);
}

void	fork_unlock(t_fork *fork, t_data *data)
{
	pthread_mutex_lock(&data->table_lock);
	fork->in_use = 0;
	pthread_mutex_unlock(&data->table_lock);
}

long	time_since_meal(t_philo *philo, long now)
{
	long	last;

	pthread_mutex_lock(&philo->meal_lock);
	last = philo->last_meal;
	pthread_mutex_unlock(&philo->meal_lock);
	return (now - last);
}

int	take_forks(t_philo *philo)
{
	t_data	*data;

	data = philo->data;
	pthread_mutex_lock(&data->table_lock);
	while (!get_dead(data) && (philo->left_fork->in_use
			|| philo->right_fork->in_use || !is_hungriest(philo)))
	{
		pthread_mutex_unlock(&data->table_lock);
		usleep(500);
		pthread_mutex_lock(&data->table_lock);
	}
	if (get_dead(data))
	{
		pthread_mutex_unlock(&data->table_lock);
		return (1);
	}
	philo->left_fork->in_use = 1;
	philo->right_fork->in_use = 1;
	pthread_mutex_unlock(&data->table_lock);
	print_status(philo, "has taken a fork");
	print_status(philo, "has taken a fork");
	return (0);
}

int	craete_fork(t_data *data)
{
	int	i;

	i = 0;
	data->forks = malloc(sizeof(t_fork) * data->num_philo);
	if (!data->forks)
		return (1);
	while (i < data->num_philo)
	{
		data->forks[i].in_use = 0;
		data->forks[i].id = i;
		i++;
	}
	return (0);
}
