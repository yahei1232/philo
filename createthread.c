#include "philo.h"

int create_threads(t_data *data)
{
    int i;

    i = 0; 
    while (i < data->num_philos)
    {
        if (pthread_create(&data->philosophers[i].thread, NULL,
            philosopher_routine, &data->philosophers[i]) != 0)
            return (0);
        i++;
    }
    return (1);
}

void join_threads(t_data *data)
{
    int i;

    for (i = 0; i < data->num_philos; i++)
        pthread_join(data->philosophers[i].thread, NULL);
}
