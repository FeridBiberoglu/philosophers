#include "philo.h"

static int init_mutexes(t_program *program, int num_philos)
{
    int i;

    program->forks = malloc(sizeof(pthread_mutex_t) * num_philos);
    if (!program->forks)
        return (1);
    i = 0;
    while (i < num_philos)
    {
        if (pthread_mutex_init(&program->forks[i], NULL))
            return (1);
        i++;
    }
    if (pthread_mutex_init(&program->write_lock, NULL))
        return (1);
    if (pthread_mutex_init(&program->dead_lock, NULL))
        return (1);
    if (pthread_mutex_init(&program->meal_lock, NULL))
        return (1);
    if (pthread_mutex_init(&program->start_lock, NULL))
        return (1);
    program->start_flag = 0;
    return (0);
}

static void assign_forks(t_program *program, int num_philos)
{
    int i;

    i = 0;
    while (i < num_philos)
    {
        program->philos[i].l_fork = &program->forks[i];
        if (i == num_philos - 1)
            program->philos[i].r_fork = &program->forks[0];
        else
            program->philos[i].r_fork = &program->forks[i + 1];
        i++;
    }
}

static int init_philos(t_program *program, char **argv)
{
    int i;
    int num_philos;

    num_philos = ft_atoi(argv[1]);
    program->philos = malloc(sizeof(t_philo) * num_philos);
    if (!program->philos)
        return (1);
    i = 0;
    while (i < num_philos)
    {
        program->philos[i].id = i + 1;
        program->philos[i].eating = 0;
        program->philos[i].meals_eaten = 0;
        program->philos[i].last_meal = get_current_time();
        program->philos[i].time_to_die = ft_atoi(argv[2]);
        program->philos[i].time_to_eat = ft_atoi(argv[3]);
        program->philos[i].time_to_sleep = ft_atoi(argv[4]);
        program->philos[i].start_time = get_current_time();
        program->philos[i].num_of_philos = num_philos;
        program->philos[i].num_times_to_eat = -1;
        if (argv[5])
            program->philos[i].num_times_to_eat = ft_atoi(argv[5]);
        program->philos[i].dead = &program->dead_flag;
        program->philos[i].write_lock = &program->write_lock;
        program->philos[i].dead_lock = &program->dead_lock;
        program->philos[i].meal_lock = &program->meal_lock;
        program->philos[i].start_lock = &program->start_lock;
        program->philos[i].program = program;
        i++;
    }
    return (0);
}

int init_program(t_program *program, int argc, char **argv)
{
    int num_philos;

    (void)argc;
    num_philos = ft_atoi(argv[1]);
    if (num_philos > 200)
        return (1);
    program->dead_flag = 0;
    if (init_mutexes(program, num_philos))
        return (1);
    if (init_philos(program, argv))
        return (1);
    assign_forks(program, num_philos);
    return (0);
}
