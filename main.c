#include "philo.h"

static int check_args(int argc, char **argv)
{
    int i;
    int j;

    if (argc != 5 && argc != 6)
        return (1);
    i = 1;
    while (argv[i])
    {
        j = 0;
        while (argv[i][j])
        {
            if (argv[i][j] < '0' || argv[i][j] > '9')
                return (1);
            j++;
        }
        if (ft_atoi(argv[i]) <= 0)
            return (1);
        i++;
    }
    return (0);
}

static void cleanup(t_program *program)
{
    int i;

    i = 0;
    while (i < program->philos[0].num_of_philos)
    {
        pthread_mutex_destroy(&program->forks[i]);
        i++;
    }
    pthread_mutex_destroy(&program->write_lock);
    pthread_mutex_destroy(&program->dead_lock);
    pthread_mutex_destroy(&program->meal_lock);
    free(program->forks);
    free(program->philos);
}

int main(int argc, char **argv)
{
    t_program   program;
    int         i;
    size_t      start_time;

    if (check_args(argc, argv))
    {
        printf("Error: Invalid arguments\n");
        return (1);
    }
    if (init_program(&program, argc, argv))
        return (1);
    
    i = 0;
    while (i < program.philos[0].num_of_philos)
    {
        if (pthread_create(&program.philos[i].thread, NULL, 
            philo_routine, &program.philos[i]))
            return (1);
        i++;
    }
    if (pthread_create(&program.monitor, NULL, monitor_routine, &program))
        return (1);

    // Set start time and release threads
    start_time = get_current_time();
    i = 0;
    while (i < program.philos[0].num_of_philos)
    {
        program.philos[i].start_time = start_time;
        program.philos[i].last_meal = start_time;
        i++;
    }
    pthread_mutex_lock(&program.start_lock);
    program.start_flag = 1;
    pthread_mutex_unlock(&program.start_lock);

    i = 0;
    while (i < program.philos[0].num_of_philos)
    {
        pthread_join(program.philos[i].thread, NULL);
        i++;
    }
    pthread_join(program.monitor, NULL);
    cleanup(&program);
    return (0);
}
