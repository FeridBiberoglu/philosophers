/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbiberog <fbiberog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 14:49:50 by fbiberog          #+#    #+#             */
/*   Updated: 2024/11/21 14:53:02 by fbiberog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	should_continue(t_philo *philo)
{
	return (!check_death(philo)
		&& (philo->meals_eaten != philo->num_times_to_eat));
}

static int	check_philo_death(t_program *program, t_philo *philo)
{
	size_t	time;

	pthread_mutex_lock(&program->meal_lock);
	time = get_current_time();
	if ((time - philo->last_meal) >= philo->time_to_die)
	{
		pthread_mutex_lock(&program->dead_lock);
		program->dead_flag = 1;
		pthread_mutex_unlock(&program->dead_lock);
		print_message(philo, "died");
		pthread_mutex_unlock(&program->meal_lock);
		usleep(1000);
		return (1);
	}
	pthread_mutex_unlock(&program->meal_lock);
	return (0);
}

static int	monitor_philos(t_program *program)
{
	int	i;

	i = 0;
	while (i < program->philos[0].num_of_philos)
	{
		if (check_philo_death(program, &program->philos[i]))
			return (1);
		i++;
	}
	return (0);
}

static int	check_death(t_philo *philo)
{
	int	is_dead;

	pthread_mutex_lock(philo->dead_lock);
	is_dead = *philo->dead;
	pthread_mutex_unlock(philo->dead_lock);
	return (is_dead);
}

static void	wait_for_start(t_philo *philo)
{
	pthread_mutex_lock(philo->start_lock);
	while (((t_program *)philo->program)->start_flag == 0)
	{
		pthread_mutex_unlock(philo->start_lock);
		usleep(100);
		pthread_mutex_lock(philo->start_lock);
	}
	pthread_mutex_unlock(philo->start_lock);
}
