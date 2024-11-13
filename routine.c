#include "philo.h"

static int check_death(t_philo *philo)
{
    pthread_mutex_lock(philo->dead_lock);
    if (*philo->dead)
    {
        pthread_mutex_unlock(philo->dead_lock);
        return (1);
    }
    pthread_mutex_unlock(philo->dead_lock);
    return (0);
}

static void philo_eat(t_philo *philo)
{
    pthread_mutex_t *first_fork;
    pthread_mutex_t *second_fork;

    if (philo->id % 2)
    {
        first_fork = philo->l_fork;
        second_fork = philo->r_fork;
    }
    else
    {
        first_fork = philo->r_fork;
        second_fork = philo->l_fork;
    }

    pthread_mutex_lock(first_fork);
    print_message(philo, "has taken a fork");
    pthread_mutex_lock(second_fork);
    print_message(philo, "has taken a fork");
    
    pthread_mutex_lock(philo->meal_lock);
    philo->last_meal = get_current_time();
    philo->eating = 1;
    print_message(philo, "is eating");
    pthread_mutex_unlock(philo->meal_lock);
    
    ft_usleep(philo->time_to_eat);
    
    pthread_mutex_lock(philo->meal_lock);
    philo->eating = 0;
    philo->meals_eaten++;
    pthread_mutex_unlock(philo->meal_lock);
    
    pthread_mutex_unlock(second_fork);
    pthread_mutex_unlock(first_fork);
}

static void wait_for_start(t_philo *philo)
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

void *philo_routine(void *arg)
{
    t_philo *philo;

    philo = (t_philo *)arg;
    wait_for_start(philo);
    if (philo->num_of_philos == 1)
    {
        pthread_mutex_lock(philo->r_fork);
        print_message(philo, "has taken a fork");
        pthread_mutex_lock(philo->meal_lock);
        philo->last_meal = get_current_time();
        pthread_mutex_unlock(philo->meal_lock);
        ft_usleep(philo->time_to_die);
        pthread_mutex_unlock(philo->r_fork);
        return (NULL);
    }
    pthread_mutex_lock(philo->meal_lock);
    philo->last_meal = get_current_time();
    pthread_mutex_unlock(philo->meal_lock);
    
    if (philo->id % 2)
        ft_usleep(philo->time_to_eat);
    
    while (!check_death(philo))
    {
        print_message(philo, "is thinking");
        ft_usleep(1);
        philo_eat(philo);
        if (philo->meals_eaten == philo->num_times_to_eat)
            break;
        print_message(philo, "is sleeping");
        ft_usleep(philo->time_to_sleep);
    }
    return (NULL);
}

static int check_meals(t_program *program)
{
    int i;
    int finished;

    if (program->philos[0].num_times_to_eat == -1)
        return (0);
    i = 0;
    finished = 0;
    while (i < program->philos[0].num_of_philos)
    {
        pthread_mutex_lock(&program->meal_lock);
        if (program->philos[i].meals_eaten >= program->philos[i].num_times_to_eat)
            finished++;
        pthread_mutex_unlock(&program->meal_lock);
        i++;
    }
    return (finished == program->philos[0].num_of_philos);
}

void *monitor_routine(void *arg)
{
    t_program *program;
    int i;
    size_t time;

    program = (t_program *)arg;
    wait_for_start(&program->philos[0]);
    while (1)
    {
        i = 0;
        while (i < program->philos[0].num_of_philos)
        {
            pthread_mutex_lock(&program->meal_lock);
            time = get_current_time();
            if (!program->philos[i].eating && 
                (time - program->philos[i].last_meal) >= program->philos[i].time_to_die)
            {
                pthread_mutex_lock(&program->dead_lock);
                if (program->dead_flag == 0)
                {
                    program->dead_flag = 1;
                    print_message(&program->philos[i], "died");
                }
                pthread_mutex_unlock(&program->dead_lock);
                pthread_mutex_unlock(&program->meal_lock);
                return (NULL);
            }
            pthread_mutex_unlock(&program->meal_lock);
            i++;
        }
        if (check_meals(program))
            return (NULL);
        usleep(100);
    }
    return (NULL);
}

// ./philo 200 410 200 200  
