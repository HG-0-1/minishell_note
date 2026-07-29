/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dead_fun_and_eat.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helfayez <helfayez@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/28 15:06:04 by helfayez          #+#    #+#             */
/*   Updated: 2026/07/29 01:35:38 by helfayez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	get_dead(t_data *data)
{
	int	dead;

	pthread_mutex_lock(&data->dead_lock);
	dead = data->dead;
	pthread_mutex_unlock(&data->dead_lock);
	return (dead);
}

int	check_death(t_philo *philo)
{
	long	current;
	long	last;
	int		eaten;

	current = get_time();
	pthread_mutex_lock(&philo->meal_lock);
	last = philo->last_meal;
	eaten = philo->meal_eaten;
	pthread_mutex_unlock(&philo->meal_lock);
	if (philo->data->must_eat > 0 && eaten >= philo->data->must_eat)
		return (0);
	if (current - last > philo->data->time_to_die)
	{
		pthread_mutex_lock(&philo->data->dead_lock);
		if (philo->data->dead)
		{
			pthread_mutex_unlock(&philo->data->dead_lock);
			return (1);
		}
		philo->data->dead = 1;
		pthread_mutex_unlock(&philo->data->dead_lock);
		pthread_mutex_lock(&philo->data->print_lock);
		printf("%ld %d died\n", current - philo->data->start_time, philo->id);
		pthread_mutex_unlock(&philo->data->print_lock);
		return (1);
	}
	return (0);
}

void	eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->meal_lock);
	philo->last_meal = get_time();
	philo->meal_eaten++;
	pthread_mutex_unlock(&philo->meal_lock);
	print_status(philo, "is eating");
	smart_sleep(philo->data->time_to_eat, philo);
	fork_unlock(philo->left_fork, philo->data);
	fork_unlock(philo->right_fork, philo->data);
}

int	is_done(t_data *data, t_philo *philo)
{
	int	eaten;

	if (data->must_eat <= 0)
		return (0);
	pthread_mutex_lock(&philo->meal_lock);
	eaten = philo->meal_eaten;
	pthread_mutex_unlock(&philo->meal_lock);
	return (eaten >= data->must_eat);
}

int	is_hungriest(t_philo *philo)
{
	t_data	*data;
	t_philo	*left;
	t_philo	*right;
	long	now;
	long	my_wait;

	data = philo->data;
	left = &data->philo[(philo->id - 2 + data->num_philo) % data->num_philo];
	right = &data->philo[philo->id % data->num_philo];
	now = get_time();
	my_wait = time_since_meal(philo, now);
	return ((is_done(data, left) || my_wait >= time_since_meal(left, now))
		&& (is_done(data, right) || my_wait >= time_since_meal(right, now)));
}
