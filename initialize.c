/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hassende <hassende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 19:33:09 by hassende          #+#    #+#             */
/*   Updated: 2024/03/03 22:40:29 by hassende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int validate_args(int argc, char **argv)
{
    int i;
    int j;

    if (argc != 5 && argc != 6)
        return (0);
    i = 1;
    while (i < argc)
    {
        j = 0;
        while (argv[i][j])
        {
            if (argv[i][j] < '0' || argv[i][j] > '9')
                return (0);
            j++;
        }
        i++;
    }
    return (1);
}

static int parse_args(t_data *data, int argc, char **argv)
{
    data->num_philos = atoi(argv[1]);
    data->time_to_die = atoi(argv[2]);
    data->time_to_eat = atoi(argv[3]);
    data->time_to_sleep = atoi(argv[4]);
    data->must_eat_count = (argc == 6) ? atoi(argv[5]) : -1;
    data->someone_died = 0;

    if (data->num_philos < 1 || data->num_philos > 200 ||
        data->time_to_die < 60 || data->time_to_eat < 60 ||
        data->time_to_sleep < 60 || 
        (argc == 6 && data->must_eat_count < 1))
        return (0);
    return (1);
}

static int init_mutexes(t_data *data)
{
    int i;

    if (pthread_mutex_init(&data->print_mutex, NULL) != 0)
        return (0);

    i = 0;
    while (i < data->num_philos)
    {
        if (pthread_mutex_init(&data->forks[i].mutex, NULL) != 0)
        {
            while (--i >= 0)
                pthread_mutex_destroy(&data->forks[i].mutex);
            pthread_mutex_destroy(&data->print_mutex);
            return (0);
        }
        data->forks[i].id = i;
        i++;
    }
    return (1);
}

static void init_philosophers(t_data *data)
{
    int i;

    i = 0;
    while (i < data->num_philos)
    {
        data->philosophers[i].id = i;
        data->philosophers[i].times_eaten = 0;
        data->philosophers[i].last_meal = get_time();
        data->philosophers[i].left_fork = &data->forks[i];
        data->philosophers[i].right_fork = &data->forks[(i + 1) % data->num_philos];
        data->philosophers[i].data = data;
        i++;
    }
}

int init_data(t_data *data, int argc, char **argv)
{
    if (!validate_args(argc, argv))
    {
        printf("Error: Invalid arguments\n");
        return (0);
    }

    if (!parse_args(data, argc, argv))
    {
        printf("Error: Invalid argument values\n");
        return (0);
    }

    data->philosophers = malloc(sizeof(t_philo) * data->num_philos);
    if (!data->philosophers)
    {
        printf("Error: Memory allocation failed\n");
        return (0);
    }

    data->forks = malloc(sizeof(t_fork) * data->num_philos);
    if (!data->forks)
    {
        free(data->philosophers);
        printf("Error: Memory allocation failed\n");
        return (0);
    }

    if (!init_mutexes(data))
    {
        free(data->philosophers);
        free(data->forks);
        printf("Error: Mutex initialization failed\n");
        return (0);
    }

    init_philosophers(data);
    return (1);
}
