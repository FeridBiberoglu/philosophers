/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbiberog <fbiberog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 14:47:49 by fbiberog          #+#    #+#             */
/*   Updated: 2024/11/21 14:53:26 by fbiberog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	start_eating(t_philo *philo)
{
	pthread_mutex_lock(philo->meal_lock);
	philo->last_meal = get_current_time();
	philo->eating = 1;
	print_message(philo, "is eating");
	pthread_mutex_unlock(philo->meal_lock);
}

static void	finish_eating(t_philo *philo, pthread_mutex_t *first,
		pthread_mutex_t *second)
{
	pthread_mutex_lock(philo->meal_lock);
	philo->eating = 0;
	philo->meals_eaten++;
	pthread_mutex_unlock(philo->meal_lock);
	pthread_mutex_unlock(second);
	pthread_mutex_unlock(first);
}

static void	philo_eat(t_philo *philo)
{
	pthread_mutex_t	*first_fork;
	pthread_mutex_t	*second_fork;

	get_forks(philo, &first_fork, &second_fork);
	take_forks(philo, first_fork, second_fork);
	start_eating(philo);
	ft_usleep(philo->time_to_eat);
	finish_eating(philo, first_fork, second_fork);
}

int	check_meals(t_program *p)
{
	int	i;
	int	finished;

	if (p->philos[0].num_times_to_eat == -1)
		return (0);
	i = 0;
	finished = 0;
	while (i < p->philos[0].num_of_philos)
	{
		pthread_mutex_lock(&p->meal_lock);
		if (p->philos[i].meals_eaten >= p->philos[i].num_times_to_eat)
			finished++;
		pthread_mutex_unlock(&p->meal_lock);
		i++;
	}
	return (finished == p->philos[0].num_of_philos);
}
