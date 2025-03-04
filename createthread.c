/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   createthread.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hassende <hassende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 19:33:09 by hassende          #+#    #+#             */
/*   Updated: 2024/03/03 22:40:29 by hassende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int check_death_flag(t_data *data)
{
    pthread_mutex_lock(&data->print_mutex);
    if (data->someone_died)
    {
        pthread_mutex_unlock(&data->print_mutex);
        return (1);
    }
    pthread_mutex_unlock(&data->print_mutex);
    return (0);
}

int create_threads(t_data *data)
{
    int i;
    pthread_t monitor;

    data->start_time = get_time();
    if (pthread_create(&monitor, NULL, (void *)monitor_philosophers, data) != 0)
    {
        printf("Error creating monitor thread\n");
        return (1);
    }

    i = 0;
    while (i < data->num_philos)
    {
        if (pthread_create(&data->philosophers[i].thread, NULL,
            philosopher_routine, &data->philosophers[i]) != 0)
        {
            printf("Error creating philosopher thread %d\n", i + 1);
            data->someone_died = 1;
            return (1);
        }
        // Stagger philosopher starts to prevent deadlock
        if (i % 2 == 0)
            usleep(100);
        i++;
    }

    // Join monitor thread
    if (pthread_join(monitor, NULL) != 0)
    {
        printf("Error joining monitor thread\n");
        return (1);
    }

    // Join philosopher threads
    i = 0;
    while (i < data->num_philos)
    {
        if (pthread_join(data->philosophers[i].thread, NULL) != 0)
        {
            printf("Error joining philosopher thread %d\n", i + 1);
            return (1);
        }
        i++;
    }
    return (0);
}

void join_threads(t_data *data)
{
    int i;

    for (i = 0; i < data->num_philos; i++)
        pthread_join(data->philosophers[i].thread, NULL);
}
