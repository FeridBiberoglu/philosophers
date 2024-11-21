/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbiberog <fbiberog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 14:21:35 by fbiberog          #+#    #+#             */
/*   Updated: 2024/11/21 14:39:22 by fbiberog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_philo
{
	pthread_t			thread;
	int					id;
	int					eating;
	int					meals_eaten;
	size_t				last_meal;
	size_t				time_to_die;
	size_t				time_to_eat;
	size_t				time_to_sleep;
	size_t				start_time;
	int					num_of_philos;
	int					num_times_to_eat;
	int					*dead;
	pthread_mutex_t		*r_fork;
	pthread_mutex_t		*l_fork;
	pthread_mutex_t		*write_lock;
	pthread_mutex_t		*dead_lock;
	pthread_mutex_t		*meal_lock;
	pthread_mutex_t		*start_lock;
	struct s_program	*program;
}						t_philo;

typedef struct s_program
{
	int					dead_flag;
	int					start_flag;
	pthread_mutex_t		dead_lock;
	pthread_mutex_t		meal_lock;
	pthread_mutex_t		write_lock;
	pthread_mutex_t		start_lock;
	pthread_mutex_t		*forks;
	t_philo				*philos;
	pthread_t			monitor;
}						t_program;

// Utils functions
size_t					get_current_time(void);
int						ft_usleep(size_t milliseconds);
int						ft_atoi(const char *str);
void					print_message(t_philo *philo, char *msg);

// Init functions
int						init_program(t_program *program, int argc, char **argv);

// Routine functions
void					*philo_routine(void *arg);
void					*monitor_routine(void *arg);
int						check_meals(t_program *program);

#endif