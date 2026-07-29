/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helfayez <helfayez@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/22 15:11:17 by helfayez          #+#    #+#             */
/*   Updated: 2026/07/29 13:08:12 by helfayez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <time.h>
# include <unistd.h>

typedef struct s_philo	t_philo;

typedef struct s_fork
{
	int					in_use;
	int					id;
}						t_fork;

typedef struct s_data
{
	int					dead;
	long				start_time;
	int					num_philo;
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					must_eat;
	int					finished_count;
	pthread_mutex_t		print_lock;
	t_fork				*forks;
	pthread_mutex_t		table_lock;
	pthread_mutex_t		dead_lock;
	pthread_mutex_t		finished_lock;
	t_philo				*philo;
}						t_data;

typedef struct s_philo
{
	int					id;
	pthread_t			thread;
	long				last_meal;
	int					meal_eaten;
	t_fork				*left_fork;
	t_fork				*right_fork;
	pthread_mutex_t		meal_lock;
	pthread_cond_t		cond; // forbidden
	t_data				*data;

}						t_philo;

int						take_forks(t_philo *philo);
void					fork_lock(t_fork *fork, t_data *data);
void					fork_unlock(t_fork *fork, t_data *data);
int						craete_fork(t_data *data);
void					*routine(void *arg);
long					get_time(void);
void					print_status(t_philo *philo, char *status);
int						check_death(t_philo *philo);
void					*monitor(void *arg);
int						get_dead(t_data *data);
void					smart_sleep(long time, t_philo *philo);
int						ft_atoi(const char *nptr);
int						is_valid_number(const char *str);
void					eat(t_philo *philo);
void					next_deadline(struct timespec *ts);
int						is_hungriest(t_philo *philo);
int						all_ate_enough(t_data *data);
int						init_data(t_data *data, t_philo **philos, int argc,
							char **argv);
int						check_args(int argc, char **argv);
long					time_since_meal(t_philo *philo, long now);
int	is_done(t_data *data, t_philo *philo);
int	parse_args(t_data *data, int argc, char **argv);


#endif