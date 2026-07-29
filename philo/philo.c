/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helfayez <helfayez@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/22 15:11:13 by helfayez          #+#    #+#             */
/*   Updated: 2026/07/29 13:08:03 by helfayez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	creat_philosopher(t_philo *philo, t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philo)
	{
		philo[i].id = i + 1;
		philo[i].data = data;
		philo[i].left_fork = &data->forks[i];
		philo[i].right_fork = &data->forks[(i + 1) % data->num_philo];
		pthread_mutex_init(&philo[i].meal_lock, NULL);
		philo[i].last_meal = data->start_time;
		philo[i].meal_eaten = 0;
		i++;
	}
	i = 0;
	while (i < data->num_philo)
	{
		if (pthread_create(&philo[i].thread, NULL, routine, &philo[i]) != 0)
			return (1);
		i++;
	}
	return (0);
}

int	join_philo(t_philo *philo, t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philo)
	{
		pthread_join(philo[i].thread, NULL);
		i++;
	}
	return (0);
}

int	init_data(t_data *data, t_philo **philos, int argc, char **argv)
{
	if (parse_args(data, argc, argv))
		return (1);
	data->dead = 0;
	pthread_mutex_init(&data->print_lock, NULL);
	pthread_mutex_init(&data->table_lock, NULL);
	pthread_mutex_init(&data->dead_lock, NULL);
	if (craete_fork(data))
		return (1);
	*philos = malloc(sizeof(t_philo) * data->num_philo);
	if (!*philos)
		return (1);
	data->philo = *philos;
	data->start_time = get_time();
	if (creat_philosopher(*philos, data))
		return (1);
	return (0);
}

int	main(int argc, char **argv)
{
	t_data		data;
	t_philo		*philos;
	pthread_t	monitor_thread;
	int			i;

	i = 0;
	if (init_data(&data, &philos, argc, argv))
		return (1);
	pthread_create(&monitor_thread, NULL, monitor, &data);
	join_philo(philos, &data);
	pthread_join(monitor_thread, NULL);
	while (i < data.num_philo)
	{
		pthread_mutex_destroy(&philos[i].meal_lock);
		i++;
	}
	pthread_mutex_destroy(&data.print_lock);
	pthread_mutex_destroy(&data.table_lock);
	pthread_mutex_destroy(&data.dead_lock);
	free(philos);
	free(data.forks);
	return (0);
}
