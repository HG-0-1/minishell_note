/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   its_true.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helfayez <helfayez@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/29 01:33:48 by helfayez          #+#    #+#             */
/*   Updated: 2026/07/29 01:36:42 by helfayez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	parse_args(t_data *data, int argc, char **argv)
{
	if (argc != 5 && argc != 6)
		return (1);
	if (check_args(argc, argv))
		return (1);
	data->num_philo = ft_atoi(argv[1]);
	if (data->num_philo <= 0)
		return (1);
	data->time_to_die = ft_atoi(argv[2]);
	if (data->time_to_die <= 0)
		return (1);
	data->time_to_eat = ft_atoi(argv[3]);
	if (data->time_to_eat <= 0)
		return (1);
	data->time_to_sleep = ft_atoi(argv[4]);
	if (data->time_to_sleep <= 0)
		return (1);
	if (argc == 6)
		data->must_eat = ft_atoi(argv[5]);
	else
		data->must_eat = -1;
	if (argc == 6 && data->must_eat <= 0)
		return (1);
	return (0);
}
int	check_args(int argc, char **argv)
{
	int	i;

	i = 1;
	while (i < argc)
	{
		if (!is_valid_number(argv[i]))
			return (1);
		i++;
	}
	return (0);
}
int	all_ate_enough(t_data *data)
{
	int	i;
	int	eaten;

	if (data->must_eat <= 0)
		return (0);
	i = 0;
	while (i < data->num_philo)
	{
		pthread_mutex_lock(&data->philo[i].meal_lock);
		eaten = data->philo[i].meal_eaten;
		pthread_mutex_unlock(&data->philo[i].meal_lock);
		if (eaten < data->must_eat)
			return (0);
		i++;
	}
	return (1);
}